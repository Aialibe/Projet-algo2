# Projet-algo2
J'ai créé ouest 06/01/2025 à 11h11

08/01/25 de 10h à 12h : MAJ du demineur avec l'ajout de la fonctionnalité pour jouer et dévoiler les cases.


15/01/2025 : Quand on clique sur un chiffre qui a tous ses drapeaux (exemple 1 et ya un drapeau dans son entourage) les case adjacentes doivent se reveler, mais ca marche pas (ex ya un 1 en (4 0) et drapeau en (5 2), en mode 5 bombes) ---> RÉSOLU 16/01/2025 : le pb c'était les atoi je suis devasté (atoi prend une chaîne de caractère pas un caractère c'est pour ça qu'il voulait un char* mais que mettre un & devant la variable ça réglait pas le pb. Du coup c'est pas giga propre mais j'ai mis des -48 (code ascii du '0') pour les vérifications)
