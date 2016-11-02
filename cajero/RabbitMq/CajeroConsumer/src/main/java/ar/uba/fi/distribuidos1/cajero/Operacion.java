package ar.uba.fi.distribuidos1.cajero;

import java.io.IOException;
import java.io.Serializable;

public abstract class Operacion implements Serializable {

	private static final long serialVersionUID = 4740707478989413332L;
	protected Integer cantidad;
	
	public Operacion(Integer cantidad) {
		this.cantidad = cantidad;
	}
	
	public abstract void operar(Cuenta c) throws IOException;
}
