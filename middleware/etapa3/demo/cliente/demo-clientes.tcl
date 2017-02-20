#!/usr/bin/expect -f


set nice_timeout 1500
set server_file "./MiddlewareServer"
set client_file "./Cliente"
set translators_count 3
set clients_count 2

####################### STARTING MIDDLEWARE


puts "################## EJECUTANDO MIDDLEWARE ######################\n"
spawn $server_file
set server_id $spawn_id
expect {
	 timeout {puts "Fin por time out"; exit}
	 "Recursos iniciados correctamente" {
	 	after $nice_timeout
	 	puts "Middleware Iniciado\n"
	 }
}

after $nice_timeout

#Must run translators script

puts "######################## ESPERANDO POR LA EJECUCIÓN DEL SCRIPT DE TRADUCTORES #############################"


set timeout 120


for { set a 0}  {$a < $translators_count} {incr a} {
	expect {
		 timeout {puts "No se ha conectado ningún traductor"; exit}
		 "Traductor conectado" {
		 	after $nice_timeout
		 	puts "Traductor conectado\n"
		 }
	}   
}



########################## PRODUCERS


puts "################## INICIANDO CLIENTES ######################\n"



set timeout 10
for {set a 0}  {$a < $clients_count} {incr a} {
	############ CLIENT
	set number [expr $a + 1 ]
	puts "Iniciando Cliente $number"
	after $nice_timeout
	spawn $client_file
	expect {
		  timeout {puts "Fin por time out"; exit}
		 "Ingrese una palabra:" {
		 	puts "\nCliente $number iniciado correctamente\n"
		 	after $nice_timeout
		 }
	}
	set clients($a) $spawn_id
	after $nice_timeout
}


################################## LOAD WORDS
set f [open "demo-palabras.txt"]
set text [read $f]
set words [split $text " "]
close $f


after $nice_timeout
puts "################ EL TEXTO A TRADUCIR ES: \"$text\" \n"
after $nice_timeout


################################## TRANSLATE

puts "################## COMENZANDO TRADUCCIÓN ######################\n"
set index 0
set accum {} 
foreach word $words {
	set index [expr {int(rand()*20) % 2}]
	set cln [expr $index + 1]
	puts "Traduciendo \"$word\" en el cliente $cln"
	send -i $clients($index) "$word\n"
	after $nice_timeout
	
	expect {
		-i $clients($index)
		timeout {exit}
		-re {Su palabra en jeringoso es: (.+)\r\n} {
			after $nice_timeout
			puts "\n\rLa traducción obtenida es \"$expect_out(1,string)\" \n"
			set accum "${accum} $expect_out(1,string)" 
		}
	}

	after $nice_timeout
}

################## PRINT ORIGINAL WORDS AND TRANSLATION
set acum [string trim $accum]
puts "La frase original es \"$text\""
puts "La traducción completa es \"$acum\""
after $nice_timeout



######################## FINISHING MIDDLEWARE

puts "\n################## FINALIZANDO EL MIDDLEWARE ######################\n"

send -i $server_id \x03

spawn ./Destructor
expect {
	 timeout {puts "Fin por time out"; exit}
	 "Recursos compartidos liberados correctamente" {puts "Middleware Finalizado\n"}
}




after $nice_timeout
puts "Fin de la presentación"

