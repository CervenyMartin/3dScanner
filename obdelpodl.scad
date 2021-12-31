// obdelnikova podlozka

x = 28;
y = 40;
z = 0.5;
polomerDer = 5;
vzdalenostDer = 25;
$fn=80;
difference(){
cube([x, y, z], center=true);
translate([0,vzdalenostDer/2,0])
    cylinder(h=3*z, r=polomerDer, center=true);
translate([0,-vzdalenostDer/2,0])    
    cylinder(h=3*z, r=polomerDer, center=true);
}