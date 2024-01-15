	Pentru implementarea functiilor din aceasta tema am folosit notiunile invatate la curs si laborator, cu 
privire la arbori AVL si liste dublu inlantuite, dar am incercat o abordare originala asupra functiilor, din moment
ce au fost mult mai complicate decat la laboratoare.
	In rezolvarea primei cerinte am utilizat cunostintele dobandite pentru a realiza functiile createTree,
search, minimum, maximum, successor, predecessor, updateHeight, avlGetBalance si destroyTree, folosindu-ma de
informatiile din cursuri si laboratoare. Pentru functiile ce realizeaza rotatiile in arbore si rebalansarea
acestuia, am creat proprii algoritmi si i-am rectificat astfel incat aceste operatii sa fie corecte. Functiile
avlFixUp, insert si delete au fost cele mai grele deoarece in cadrul laboratorului am invatat sa le implementez
doar cand acestea returneaza un nod, dar abordarea de tip void a fost mult mai grea.
	Pentru cerintele de criptare din tema am folosit un buffer despartit in cuvinte pentru fiecare linie si
am inserat fiecare cuvant in arbore, rebalansandu-l de fiecare data. Apoi, pe baza acestui arbore, am creat cheile
de criptare aferente functiilor inorderKeyQuery, levelKeyQuery (pentru care am avut nevoie de o functie ajutatoare
cu care am calculat nivelul fiecarui nod si am retinut valoarea intr-un vector la indexul corespunzator) si
rangeKeyQuery.
	Aceasta tema mi s-a parut foarte interesanta si am invatat foarte multe lucruri noi, mai ales in domeniul
structurilor de date abstracte, dar si in domeniile criptografiei, lucrului cu memoria si importanta gandirii
logice in programare.