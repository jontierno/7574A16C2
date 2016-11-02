package ar.uba.fi.distribuidos1.commons;

import java.io.IOException;

public class Deposito extends Operacion {


	private static final long serialVersionUID = 1321180812993511343L;

	public Deposito(Integer cantidad) {
		super(cantidad);
	}

	@Override
	public void operar(Cuenta c) throws IOException {
		c.depositar(this.cantidad);
		
	}
	
}
