# 1WireDriver

Driver pour le protocole 1-Wire créé par Maxim Integrated.
Ce driver fonctionne pour les capteurs DS1822 et DS18B20 mais probablement pour d'autres capteurs. 
Il peut facilement être réadapté si un capteur utilise d'autres code de fonctions.
Le driver a été conçu pour fonctionner sur les cartes STM32 et plus spécifiquement le Nucleo-F042K6.

En l'état actuel le driver n'est pas à 100% terminé. Il manque la fonction SearchROM et AlarmSearch qui sont les deux basées sur un algorithme d'identifications des capteurs présents sur la ligne 1-Wire.
La fonction ReadROM n'a pas pu être entièrement testée en laboratoire car après mesures le capteur retournait la valeur 0xffffffffffffffff (64 bits).

Le driver a été créé par moi à l'ETML dans le cadre d'utiliser ce protocole dans de futurs projets de l'école.
