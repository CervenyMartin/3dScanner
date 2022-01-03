a = 41;
b = 180;
tloustkaOkraju = 4;
vyskaDesticky=2.5;

translate([0,0,-vyskaDesticky/2])
difference(){
cube([a+2*tloustkaOkraju, b, vyskaDesticky],center = true);
cube([a,b,vyskaDesticky+0.01], center = true);
}


vyskaMatky = 9.2;
difference(){
translate([0,0,vyskaMatky/2])
cube([a+2*tloustkaOkraju,b,vyskaMatky],center = true);
// nohy
 polomerVelke= 20/2;

translate([0,76.5+0.75,0]) cylinder(h=8.7,r=polomerVelke, $fn=6);
translate([0,-76.5-0.75,0])cylinder(h=8.7,r=polomerVelke, $fn=6);

// stred
polomerMale = 13/2;
translate([12.5,0,0])cylinder(h=vyskaMatky, r = polomerMale+1.5,$fn = 50);
translate([-12.5,0,0])cylinder(h=vyskaMatky, r = polomerMale+1.5,$fn = 50);
// na motor
polomerMensi = 9.5/2;
translate([12.5,36.5+0.75,0])cylinder(h=6, r=polomerMensi,$fn=6);
translate([12.5,-36.5-0.75,0])cylinder(h=6, r=polomerMensi,$fn=6);
    
// na plech

translate([14,165/2,0])cylinder(h=vyskaMatky, r=5.3/2,$fn=50);
translate([14,-165/2,0])cylinder(h=vyskaMatky, r=5.3/2,$fn=50);

// prichyceni desticky
translate([12.5,63.5+0.75,3])cylinder(h=vyskaMatky, r = polomerMensi,$fn = 6);
translate([12.5,-63.5-0.75,3])cylinder(h=vyskaMatky, r = polomerMensi,$fn = 6);
translate([-12.5,63.5+0.75,3])cylinder(h=vyskaMatky, r = polomerMensi,$fn = 6);
translate([-12.5,-63.5-0.75,3])cylinder(h=vyskaMatky, r = polomerMensi,$fn = 6);

translate([12.5,63.5+0.75,0])cylinder(h=vyskaMatky, r = 5.3/2,$fn = 50);
translate([12.5,-63.5-0.75,0])cylinder(h=vyskaMatky, r = 5.3/2,$fn = 50);
translate([-12.5,63.5+0.75,0])cylinder(h=vyskaMatky, r = 5.3/2,$fn = 50);
translate([-12.5,-63.5-0.75,0])cylinder(h=vyskaMatky, r = 5.3/2,$fn = 50);
}


