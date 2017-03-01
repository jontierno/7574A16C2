#!/usr/bin/expect -f


set nice_timeout 1500

####################### STARTING MIDDLEWARE


puts "################## EJECUTANDO MIDDLEWARE ######################\n"
spawn ./build/Middleware
expect {
	 timeout {puts "Fin por time out"; exit}
	 "Recursos iniciados correctamente" {puts "Middleware Iniciado\n"}
}

after $nice_timeout


######################## TRANSLATORS

puts "################## INICIANDO TRADUCTORES ######################\n"

############ TRANSLATOR 1
puts "Iniciando Traductor"
after $nice_timeout
spawn ./build/Traductor
expect {
	  timeout {puts "Fin por time out"; exit}
	 "Iniciando Traductor" {
	 	puts "Traductor 1 iniciado correctamente\n"
	 	after $nice_timeout
	 }
}
set transl(0) $spawn_id
after $nice_timeout


############ TRANSLATOR 2
puts "Iniciando Traductor"
after $nice_timeout
spawn ./build/Traductor
expect {
	  timeout {puts "Fin por time out"; exit}
	 "Iniciando Traductor" {
	 	after $nice_timeout
	 	puts "Traductor 2 iniciado correctamente\n"
	 }
}
set transl(1) $spawn_id
after $nice_timeout


############ TRANSLATOR 3
puts "Iniciando Traductor"
after $nice_timeout
spawn ./build/Traductor
expect {
	  timeout {puts "Fin por time out"; exit}
	 "Iniciando Traductor" {
	 	after $nice_timeout
	 	puts "Traductor 3 iniciado correctamente\n"
	 }
}
set transl(2) $spawn_id
after $nice_timeout


########################## CLIENTS


puts "################## INICIANDO CLIENTES ######################\n"

############ CLIENT 1
puts "Iniciando Cliente"
after $nice_timeout
spawn ./build/Cliente
set clients(0) $spawn_id
expect {
	  timeout {puts "Fin por time out"; exit}
	 "Ingrese una palabra:" {
	 	after $nice_timeout
	 	puts "\nCliente 1 iniciado correctamente\n"
	 }
}
after $nice_timeout


############ CLIENT 2
puts "Iniciando Cliente"
after $nice_timeout
spawn ./build/Cliente
set clients(1) $spawn_id
expect {
	  timeout {puts "Fin por time out"; exit}
	 "Ingrese una palabra:" {
	 	after $nice_timeout
	 	puts "\nCliente 2 iniciado correctamente\n"
	 }
}
after $nice_timeout



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
	puts "Determinando traductor"
	after $nice_timeout
	expect {
		-i $transl(0)
		"Trabajo traducido a" {
			after $nice_timeout
			puts "Palabra traducida por el Traductor 1 \n"
		}
		-i $transl(1)
		"Trabajo traducido a" {
			after $nice_timeout
			puts "Palabra traducida por el Traductor 2 \n"
		}
		-i $transl(2)
		"Trabajo traducido a" {
			after $nice_timeout
			puts "Palabra traducida por el Traductor 3 \n"
		}

	}
	after $nice_timeout
	after $nice_timeout
	#set index [expr [expr $index + 1] % 2]	
}

################## PRINT ORIGINAL WORDS AND TRANSLATION
set acum [string trim $accum]
puts "La frase original es \"$text\""
puts "La traducción completa es \"$acum\""
after $nice_timeout



######################## FINISHING MIDDLEWARE

puts "\n################## FINALIZANDO EL MIDDLEWARE ######################\n"
spawn ./build/Destructor
expect {
	 timeout {puts "Fin por time out"; exit}
	 "Recursos compartidos liberados correctamente" {puts "Middleware Finalizado\n"}
}

after $nice_timeout
puts "Fin de la presentación"

