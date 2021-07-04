package p2p;

import java.io.EOFException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.ArrayList;


public class PeerConnection extends Thread {

    private final Socket socket;
    private WhiteBoard whiteBoard;
    private ObjectInputStream in;
    private ObjectOutputStream out;
    private String host;
    private int port;
    private boolean exit;

    /**
     * 
     * @param whiteBoard
     * @param socket
     * @param host
     * @param port
     * @throws IOException
     */
    PeerConnection(WhiteBoard whiteBoard, Socket socket, String host, int port) throws IOException {
        this.whiteBoard = whiteBoard;
        this.socket = socket;
        this.host = host; 
        this.port = port;
        this.exit = false;
        this.out = new ObjectOutputStream(this.socket.getOutputStream());
        this.out.flush(); // handshake to start the input stream of the remote peer 
        this.in = new ObjectInputStream(this.socket.getInputStream());
    }
    /**
     * @param editRecord
     * @throws IOException
     */
    public void sendEditRecord(EditRecord editRecord) throws IOException {
        this.out.writeObject(editRecord);
        this.out.flush();
    }
    /**
     * @return     A list of addresses to Peer of the current network. 
     * @throws IOException
     */
    public String[] getPeerAddressListAndEditRecord() throws IOException {
        this.out.writeInt(1); // tell the first Peer you need all data
        this.out.flush();
        try {
            String[] addressList = (String[]) this.in.readObject();
            EditRecord[] editRecord = (EditRecord[]) this.in.readObject();
            this.whiteBoard.addAllEditRecord(editRecord);
            return addressList;
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
            return null;
        }
    }
    /**
     * 
     * @throws IOException
     */
    public void sendPeerAddressListAndEditRecord() throws IOException {
        ArrayList<PeerConnection> pcList = this.whiteBoard.getPeerConnections(); 
        String[] addressList = new String[pcList.size()]; 
        for (int i = 0; i < pcList.size(); i++) {
            addressList[i] = pcList.get(i).getPeerAddress(); 
        }
        this.out.writeObject(addressList);
        this.out.flush();
        EditRecord[] er = this.whiteBoard.getRecord().toArray(new EditRecord[0]);
        this.out.writeObject(er);
        this.out.flush();
	}
    /**
     * @return  get address as Host and Port in one String
     */
    public String getPeerAddress() {
        String addressAndPort = String.format("%s %d", this.host, this.port);
        return addressAndPort;
    }

    @Override
    public void run() {
        EditRecord editRecord;
        try {
            while (!this.exit) { 
                editRecord = (EditRecord) in.readObject();
                if (editRecord == null){
                    out.writeObject(null);
                    out.flush();
                    close(); 
                    break; // if socket closes 
                }
                this.whiteBoard.addEditRecord(editRecord);
            }
            this.in.close();
            this.out.close();
            this.socket.close();
        } catch (EOFException e) {
            close();
        } catch (ClassNotFoundException e) {
            close(); 
            e.printStackTrace();
        } catch (IOException e) {
            close();
            e.printStackTrace();
        }
    }
    /**
     * receives the signal that was send by remote Peer
     * @see     sendSignalToPeer
     * @return
     * @throws IOException
     */
	public int getSignalFromPeer() throws IOException {
        return this.in.readInt();
    }
    /**
     * send signal 1 to demand all information a the Network after newly established connection. 
     * send signal 0 to let the peer know that no information is needed. 
     * @param signal
     * @throws IOException
     */
	public void sendSignalToPeer(int signal) throws IOException {
        this.out.writeInt(signal);
        this.out.flush();
    }
    /**
     * @return  Peer id of remote Peer 
     * @see     sendPeerId
     * @throws IOException
     */
	public int receivePeerId() throws IOException {
		return this.in.readInt();
    }
    /**
     * @param peerId
     * @throws IOException
     */
	public void sendPeerId(int peerId) throws IOException {
        this.out.writeInt(peerId);
        this.out.flush();
    }
    /**
     * receive port of remote Peer 
     * @throws IOException
     */
    public void receivePort() throws IOException {
		this.port = this.in.readInt();
    }
    /**
     * @param port
     * @throws IOException
     */
    public void sendPeerPort(int port) throws IOException {
        this.out.writeInt(port);
        this.out.flush();
    }
    /**
     * @throws IOException
     */
	public void stopConnection() throws IOException {
        this.out.writeObject(null);
        this.out.flush();
        this.exit = true;
    }
    /**
     * removes PeerConnection from list when socket dies
     */
    private void close() {
        this.exit = true;
        this.whiteBoard.removePeerConnection(this); 
        System.out.println(String.format("removed PeerConnection: %s %d", this.host, this.port));
    }
}