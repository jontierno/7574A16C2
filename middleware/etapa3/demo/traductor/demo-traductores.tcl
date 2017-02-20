#!/usr/bin/expect -f
set arg1 [lindex $argv 0]
set arg2 [lindex $argv 1]


set settingsPath "./settings"
set translator_file "./Traductor"
set translators_count 3


set nice_timeout 1500


if {$arg1 == ""} {
	puts "El primer argumento del script debe ser la ip del servidor\nAbandonado script"
	exit
}

if {$arg2 == ""} {
	puts "El segundo argumento debe ser la cantidad de palabras que contiene la frase\nAbandonado script"
	exit
}

###################### SETTING IP FILE
puts "Conectando los traductores a la dirección $arg1"
set settingsFile [open $settingsPath w]
puts $settingsFile $arg1
close $settingsFile



######################## TRANSLATORS



puts "################## INICIANDO TRADUCTORES ######################\n"


for {set a 0}  {$a < $translators_count} {incr a} {
	############ TRANSLATOR
	set number [expr $a + 1 ]
	puts "Iniciando Traductor $number"
	after $nice_timeout
	spawn $translator_file
	expect {
		  timeout {puts "Fin por time out"; exit}
		  "FIN DEL PROCESO" {
		  		puts "Error conectando\n"
		  		exit
		  }
		  "Se lanza un Client MiddleWare" {
		 	puts "Traductor $number iniciado correctamente\n"
		 	after $nice_timeout
		 }
	}
	set transl($a) $spawn_id
	after $nice_timeout
}




set timeout 30

for {set a 0}  {$a < $arg2} {incr a} {
	expect {
		-i $transl(0)
		timeout {exit}
		"Trabajo traducido a" {
			puts "Palabra traducida por el Traductor 1\n"
		}

		-i $transl(1)
		timeout {exit}
		"Trabajo traducido a" {
			puts "Palabra traducida por el Traductor 2\n"
		}
		-i $transl(2)
		timeout {exit}
		"Trabajo traducido a" {
			puts "Palabra traducida por el Traductor 2\n"
		}
	}
}


after $nice_timeout

puts "Liberando recursos de traductores\n"
foreach n [array names transl] {
	send -i $transl($n) \x03
}

after $nice_timeout
puts "Fin de la presentación\n"