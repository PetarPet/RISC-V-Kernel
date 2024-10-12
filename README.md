# RISC-V-Kernel
Cilj ovog projekta jeste realizacija malog, ali sasvim funkcionalnog jezgra operativnog sistema koji podržava niti (multithreaded operating system).
U okviru ovog projekta je realizovan alokator memorije i upravljanje nitima. 
Jezgro obezbedjuje koncept niti, semafora i podršku deljenju vremena, kao i asinhronu promenu konteksta i preotimanje (preemption) na prekid od
tajmera i od tastature. Jezgro je realizovano kao „bibliotečno“, tako da korisnički program (aplikacija) i 
samo jezgro dele isti adresni prostor, odnosno predstavljaju statički povezan
jedinstven program unapred učitan u operativnu memoriju računara. Konkurentni procesi
kreirani unutar aplikacije su zapravo samo laki procesi, tj. niti (thread) pokrenuti
unutar tog programa. Ovakva konfiguracija karakteristična je za embedded
sisteme, koji ne izvršavaju proizvoljne programe koji se učitavaju i izvršavaju na zahtev
korisnika, već izvršavaju samo onaj program (zajedno sa operativnim sistemom) koji je već
ugrađen u ciljni hardver.
Jezgro se implementira za arhitekturu procesora RISC-V.
