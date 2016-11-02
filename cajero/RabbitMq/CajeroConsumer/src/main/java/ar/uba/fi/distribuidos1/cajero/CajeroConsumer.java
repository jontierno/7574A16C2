package ar.uba.fi.distribuidos1.cajero;

import ar.uba.fi.distribuidos1.commons.Constantes;
import com.rabbitmq.client.*;

import java.io.IOException;

public class CajeroConsumer {
	
	private static String operar (String[] operacion) {
		return String.format(Constantes.ANSWER_SIMPLE_TEMP,Constantes.ANSWER_CODE_SUCCESS,0);
	}
	
	
	public static void main(String[] args) throws Exception {
		ConnectionFactory factory = new ConnectionFactory();
		factory.setHost("127.0.0.1");
		factory.setPort(5672);
		Connection conn = factory.newConnection();
		Channel channel = conn.createChannel();

		boolean durable = true;
		channel.exchangeDeclare(Constantes.EXCHANGE_NAME , "direct", durable);
		channel.queueDeclare(Constantes.QUEUE_NAME, durable, false, false, null);

		//Escucho todos los mensajes que sean una operacion.
		channel.queueBind(Constantes.QUEUE_NAME, Constantes.EXCHANGE_NAME, Constantes.ROUTING_KEY_OP);

		QueueingConsumer consumer = new QueueingConsumer(channel);
		System.out.println("Servidor listo para admitir mensajes");
		channel.basicConsume(Constantes.QUEUE_NAME, true, consumer);
		boolean runInfinite = true;
		while (runInfinite) {

			QueueingConsumer.Delivery delivery;
			try {
				delivery = consumer.nextDelivery();
				String body = new String(delivery.getBody());
				System.out.format("Recibido %s\n", body);
				String[] split = body.split(Constantes.MESSAGE_DELIMITER);
				String routingKey = String.format(Constantes.ANSWER_ROUTING_TEMP, split[0]);
				String answer = operar(split);
				System.out.println(Constantes.EXCHANGE_NAME+split[0] + "\n");
				channel.basicPublish(Constantes.EXCHANGE_NAME+split[0], routingKey,
						MessageProperties.PERSISTENT_TEXT_PLAIN, answer.getBytes());
			} catch (InterruptedException ie) {
				continue;
			}
		}
		channel.close();
		conn.close();
	}
}