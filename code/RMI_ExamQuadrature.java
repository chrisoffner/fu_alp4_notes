import java.io.Serializable;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class NumericalIntegrationImpl extends UnicastRemoteObject implements NumericalIntegrationIntf {

    public NumericalIntegrationImpl() throws RemoteException {
        super(0);
    }

    public double quadrature(Function f, double a, double b) throws RemoteException {
        int N = 1000; // Could also be passed as an argument
        double base = (b - a) / N;
        double sum = 0;
        for (int i = 1; i <= N; i++) {
            double x = a + base * i;
            sum += f.f(x) * base;
        }
        return sum;
    }

    public static void main(String[] args) {
        Registry registry = LocateRegistry.createRegistry(1099);
        NumericalIntegrationImpl integrator = new NumericalIntegrationImpl();
        registry.rebind("NumIntegrator", integrator);
    }
}

public interface Function {
    public double f(double x);
}

public class F implements Function, Serializable {
    public double f(double x) {
        return x; // actually do something more interesting
    }
}

public class Client {
    public static void main(String[] args) {
        // Create or prepare arguments a, b, and f

        NumericalIntegrationImpl integrator = (NumericalIntegrationImpl) Naming
                .lookup("rmi://localhost:1099/NumIntegrator");
        System.out.println(integrator.quadrature(F, a, b));
    }
}

public interface NumericalIntegrationIntf extends Remote {
    public double quadrature(double f, double a, double b) throws RemoteException;
}

// Eine mit RMI fernaufrufbare Java-Methode `quadrature` mit
// Parametern`f`,`a`,`b` auf dem Server X berechne das bestimmte Integral
// von`f(x)` über dem Intervall `[a, b]` mit einem geeigneten numerischen
// Verfahren. Die `f`-Werte sollen lokal – also auf X – berechnet werden, auch
// wenn `quadrature` von einem anderen Rechner Y aus fernaufgerufen wird.