polomerHridele = 5.09/2;
okraj = 1;
delkaNozicky=26;
vyskaPlosinky=1;
polomerPlosinky = 15/2;
$fn=300;

translate([0,0,vyskaPlosinky])
difference(){
cylinder(h=delkaNozicky,r=polomerHridele+okraj);
    translate([0,0,10])
cylinder(h=delkaNozicky,r1=polomerHridele-0.1,r2=polomerHridele);
}

cylinder(h=vyskaPlosinky, r=polomerPlosinky);