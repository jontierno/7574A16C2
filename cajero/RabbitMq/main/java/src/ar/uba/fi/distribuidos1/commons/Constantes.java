package ar.uba.fi.distribuidos1.commons;

public interface Constantes {
	
	public static final String ROUTING_KEY_OUT = "operacion";
	public static final String EXCHANGE_NAME = "myExchange";
	public static final String QUEUE_NAME = "myQueue";
	public static final String OPERACION_CREAR = "C";
	public static final String OPERACION_SALDO = "S";
	public static final String OPERACION_DEPOSITAR = "D";
	public static final String OPERACION_EXTRAER = "E";
	public static final String OPERACION_MOVIMIENTOS = "M";
	public static final String MESSAGE_DELIMITER = ";";
	// UUID; OPERACION
	public static final String MESSAGE_BASE_TEMP = "{}" + MESSAGE_DELIMITER + "{}";
	//UUID;OPERACION;CUENTA
	public static final String MESSAGE_ONLY_ACCOUNT_TEMP = MESSAGE_BASE_TEMP + MESSAGE_DELIMITER +"{}";
	//UUID;OPERACION;CUENTA;MONTO
	public static final String MESSAGE_ACC_AM_TEMP = MESSAGE_ONLY_ACCOUNT_TEMP + MESSAGE_DELIMITER +"{}";
	
	
	public static final String ANSWER_ROUTING_TEMP = "respuesta-{}";
	public static final String ANSWER_SIMPLE_TEMP = "{}"+MESSAGE_DELIMITER+"{}";
	
	
	public static final String ANSWER_CODE_SUCCESS = "0";
	public static final String ANSWER_CODE_CUENTA_EXISTENTE = "1";
	public static final String ANSWER_CODE_CUENTA_INEXISTENTE = "2";
	public static final String ANSWER_CODE_SALDO_INSUF= "3";
	
	
}
