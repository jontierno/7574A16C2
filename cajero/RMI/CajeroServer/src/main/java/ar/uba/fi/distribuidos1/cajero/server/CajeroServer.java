package ar.uba.fi.distribuidos1.cajero.server;

import java.rmi.AlreadyBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import ar.uba.fi.distribuidos1.cajero.interf.Constant;

public class CajeroServer {

	public static void main(String[] args) throws RemoteException, AlreadyBoundException {
		CajeroRemoteImpl impl = new CajeroRemoteImpl();
		Registry registry=LocateRegistry.createRegistry(Constant.RMI_PORT);
        registry.bind(Constant.RMI_ID, impl);
        System.out.println("Server is running");
	}

}
