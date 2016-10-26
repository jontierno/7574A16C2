package ar.uba.fi.distribuidos1.cajero.server;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.List;

import ar.uba.fi.distribuidos1.cajero.interf.Movimiento;
import ar.uba.fi.distribuidos1.cajero.interf.CajeroRemote;
import ar.uba.fi.distribuidos1.cajero.interf.cuenta.CuentaInexistenteException;
import ar.uba.fi.distribuidos1.cajero.interf.operacion.Operacion;

public class CajeroRemoteImpl extends UnicastRemoteObject implements CajeroRemote {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private Cajero cajero;

	protected CajeroRemoteImpl() throws RemoteException {
		super();
		this.cajero = new Cajero();
	}

	@Override
	public Integer operar(Integer id, Operacion op) throws RemoteException {
		cajero.operar(id, op);
		return cajero.consultaSaldo(id);
		
	}

	@Override
	public Integer crear(Integer id) throws CuentaInexistenteException,RemoteException {
		cajero.crear(id);
		return cajero.consultaSaldo(id);
	}

	@Override
	public Integer consultaSaldo(Integer id) throws RemoteException {
		return cajero.consultaSaldo(id);
	}

	@Override
	public List<Movimiento> consultarMovimientos(Integer id)
			throws RemoteException {
		
		return cajero.getMovimientos(id);
	}


}
