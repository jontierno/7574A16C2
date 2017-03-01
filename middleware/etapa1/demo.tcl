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

puts "################## INICIANDO TRADUCTOR ######################\n"

############ TRANSLATOR
after $nice_timeout
spawn ./build/Traductor
expect {
	  timeout {puts "Fin por time out"; exit}
	 "Iniciando Traductor" {
	 	puts "Traductor iniciado correctamente\n"
	 	after $nice_timeout
	 }
}
set transl $spawn_id
after $nice_timeout


########################## CLIENTS


puts "################## INICIANDO CLIENT ######################\n"

############ CLIENT
after $nice_timeout
spawn ./build/Cliente
set client $spawn_id
expect {
	  timeout {puts "Fin por time out"; exit}
	 "Ingrese una palabra:" {
	 	after $nice_timeout
	 	puts "\nCliente iniciado correctamente\n"
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
	puts "Traduciendo \"$word\""
	send -i client "$word\n"
	after $nice_timeout
	
	expect {
		-i client
		timeout {exit}
		-re {Su palabra en jeringoso es: (.+)\r\n} {
			after $nice_timeout
			puts "\n\rLa traducción obtenida es \"$expect_out(1,string)\" \n"
			set accum "${accum} $expect_out(1,string)" 
		}
	}

	after $nice_timeout
	puts "Verificando traductor"
	after $nice_timeout
	expect {
		-i transl
		"Trabajo traducido a" {
			after $nice_timeout
			puts "Palabra traducida en el traductor correctamente \n"
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
spawn ./build/Destructor
expect {
	 timeout {puts "Fin por time out"; exit}
	 "Recursos compartidos liberados correctamente" {puts "Middleware Finalizado\n"}
}

after $nice_timeout
puts "Fin de la presentación"

