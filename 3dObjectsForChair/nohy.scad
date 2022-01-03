polomerMatka = 20/2;
vyskaMatka = 9;
okraj=4;
vyskaMezi = 18.5;
polomerSroubu = 10/2;
difference(){
cylinder(r=polomerMatka+okraj, h=vyskaMatka, $fn = 6);
cylinder(r=polomerMatka, h = vyskaMatka, $fn = 6);
}
translate([0,0,vyskaMatka])
difference(){

cylinder(r=polomerMatka+okraj, h = vyskaMezi , $fn = 6);
cylinder(r=polomerSroubu, h = vyskaMezi, $fn = 80);
}


horniR = 15;
difference(){
hull(){
translate([0,0,vyskaMatka+vyskaMezi-10])
cylinder(r=polomerMatka+okraj, h = 1, $fn = 6);
translate([0,0,vyskaMatka+vyskaMezi-1])
cylinder(r=horniR, h = 1, $fn = 50);
}
translate([0,0,vyskaMatka])
cylinder(r=polomerSroubu, h = vyskaMezi, $fn = 80);
}

