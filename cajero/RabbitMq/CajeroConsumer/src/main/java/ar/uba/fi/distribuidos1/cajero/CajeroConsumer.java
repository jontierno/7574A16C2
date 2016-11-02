package ar.uba.fi.distribuidos1.cajero;
import ar.uba.fi.distribuidos1.commons.Constantes;

import com.rabbitmq.client.*;

public class CajeroConsumer {
	
	private static String operar (String[] operacion) {
		return "";
	}
	
	
	public static void main(String[] args) throws Exception {
		ConnectionFactory factory = new ConnectionFactory();
		factory.setHost("127.0.0.1");
		factory.setPort(5672);
		Connection conn = factory.newConnection();
		Channel channel = conn.createChannel();
		String exchangeName = "myExchange";
		String queueName = "myQueue";
		boolean durable = true;
		channel.exchangeDeclare(exchangeName, "direct", durable);
		channel.queueDeclare(queueName, durable, false, false, null);
		channel.queueBind(queueName, exchangeName, Constantes.ROUTING_KEY_OUT);
		
		boolean noAck = false;
		QueueingConsumer consumer = new QueueingConsumer(channel);
		channel.basicConsume(queueName, noAck, consumer);
		boolean runInfinite = true;
		while (runInfinite) {
			QueueingConsumer.Delivery delivery;
			try {
				delivery = consumer.nextDelivery();
				String body = new String(delivery.getBody());
				String[] split = body.split(Constantes.MESSAGE_DELIMITER);
				String routingKey = String.format(Constantes.ANSWER_ROUTING_TEMP, split[0]);
				String answer = operar(split);
				channel.basicPublish(Constantes.EXCHANGE_NAME, routingKey,
						MessageProperties.PERSISTENT_TEXT_PLAIN, answer.getBytes());
			} catch (InterruptedException ie) {
				continue;
			}
			System.out.println("Message received"
					+ new String(delivery.getBody()));
			channel.basicAck(delivery.getEnvelope().getDeliveryTag(), false);
		}
		channel.close();
		conn.close();
	}
}