# Fredj_Lab3
# Lab 3: “Accelerometer display” 

# **Introduction**

Le Lab3 est le derniére TP du co-design avec DE-10-lite, c'est le synthése de tous les autres Labs, où j'ai appris à utiliser les timers, l'interruttion et les INPUTS/OUTPUTS. Dans ce TP, j'ai implémenté une architecture qui va accéder aux valeurs des 3 axes de l'accéléromètre (les calibrer aussi) et les afficher sur les 7-segments toutes les 1 seconde.

# **Architecture**

l'architecture est composée par les composants par défaut comme d'habitude (NIOS 2 CPU, JTAG UART pour le débogage et mémoire RAM) .
J'ai utilisé 3 PIOs : 
PIO de 16 bits qui va commander les 4 7-segments pour afficher les valeurs de chaque axe ( varie entre -2000mg à 2000mg).
PIO de 4 bits qui va commander le symbole négatif sur le dernier 7-segments.
PIO de 1 bit qui recevra l'état du bouton poussoir et générera une interruption à chaque fois que le PB sera actionné.
Pour la communication avec l'accélérateur ADXL345, j'ai utilisé l'OpenCoresI2C (Implémentation libre et gratuite du protocole I2C).
Le timer va générer une interruption toutes les 1 seconde pour synchroniser la communication I2C. Plus tard, pour passer de 4bits à 8 bits pour chaque 7-segments, j'ai écrit un programme VHDL qui sera ensuite traduit en BCD dans le programme main.c en Logiciel. 

![image](https://user-images.githubusercontent.com/84923813/213141013-c267e7b0-f517-4c9d-9aac-d4ac47563af7.png)

Toutes les connexions et interruptions sur "platform designer" sont décrites dans cette figure : 

![image](https://user-images.githubusercontent.com/84923813/213142023-4ba51c1b-85b0-4688-9118-39d303251550.png)

# **Progrès**

Tout d'abord, j'ai commencé par mettre en œuvre uniquement l'accéléromètre. J'ai acquis toutes les valeurs des axes et les ai affichées sur le terminal nio2.exe comme décrit dans cette figure. 

![image](https://user-images.githubusercontent.com/84923813/213142759-2aeb46d9-4cce-4b5a-bc42-e88ee2b6193a.png)

Notez que pour utiliser la communication I2C nous avons besoin de 4 signaux : le i2c_clock (100 khz) le i2c_data et not(CS) doivent être sur le haut et ALT_adress aussi, donc nous utilisons la communication I2c sur l'adresse 0x1D.


Comme il est montré, toutes les valeurs sont fausses et n'ont aucun sens, donc j'ai dû écrire une fonction Write_I2c qui enverra les valeurs d'OFFSET de chaque axe à son registre ( OFSX, OFSY et OFSZ) pour calibrer l'ADXL345. Afin de trouver la valeur d'offset, j'ai dû utiliser la formule dans cette figure : 

![image](https://user-images.githubusercontent.com/84923813/213143203-4a027e5b-b5bc-4e65-8c3e-d6624b37feaa.png)

Cette formule ne me donnait pas la bonne valeur souhaitée, j'ai donc dû la modifier une par une et essayer de la calibrer au maximum pour respecter les valeurs données dans la fiche technique : 
![image](https://user-images.githubusercontent.com/84923813/213143798-35d7672b-b940-49bd-8bab-4988c26dc177.png)

Dans cette figure, ce sont les valeurs que j'ai obtenues lorsque l'accéléromètre est orienté vers le haut : l'axe x et l'axe y sont proches de zéro tandis que le Z est de 900 (proche de 1g).
![image](https://user-images.githubusercontent.com/84923813/213143956-1101e90b-9ef9-4d95-8a01-9c63748437a9.png)

Après le calibrage, il ne me restait plus qu'à ajouter le bouton poussoir, le timer et les 5 7-segments, en mode interruption comme dans les Labs précédents. Ainsi, à chaque interruption du timer (1 seconde), une communication I2C sera lancée.
Dans le hardware, j'ai écrit 2 fonctions VHDL : une pour commander un 7-segment normal pour afficher un nombre et une autre pour commander le 7-segment du symbole négatif pour afficher le signe négatif quand la valeur de l'axe est négative.

Cette vidéo montre le travail résultant à la fin.

https://user-images.githubusercontent.com/84923813/213145709-4d67c9b6-0e44-4f40-a942-a45a3f15435b.mp4

Notez que le programme ne fonctionne que lorsque le terminal JTAG est ouvert (lorsqu'il est fermé, le JTAG est plein et bloque toutes les autres instructions).

# **Conclusion**

Ces 3 Labs m'ont aidé à comprendre le concept de co-design, en particulier comment créer mon matériel, le connecter, programmer ses interruptions et communiquer en utilisant I2C. J'ai eu beaucoup de difficultés comme l'ajustement du symbole 7-segment, le calibrage et le débogage avec JTAG, je n'ai rien obtenu en utilisant la fonction alt_printf() qui n'était pas capable d'afficher les nombres entiers, donc j'ai migré vers la fonction printf() qui consomme plus de mémoire, donc j'ai dû augmenter la RAM pour être capable de lire les valeurs.

