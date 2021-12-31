distanceX = 74.;
distanceY = 99;
rKrajniSroub = 5.15/2;
okrajRohu = 4;


stranaKrabice = 48;
vyskaKrabice = 28+6.2;
vyskaURohu=12.5+2.5;
rohyDolu = 17;
$fn = 100;
polomerStred = 8;
vyskaStred = 1;

polomerHridele = 9/2;
stranaMotoru = 42;
uhloprickaMotoru=53.8;
vyskaMotoru = 28;
sroubyMotorDist = 31;
polomerStredKolo=25/2;
vyskaStredKolo=2.3;
rSroubyMotor = 3.12/2;
kabelVyska=12;
kabelSirka=17;
uhloprickaKrabice=sqrt(2)*stranaKrabice-2.4;


rHlavicka=6.1 /2;
rKrytka=rHlavicka+2;
hHlavicka=2.5;

module plna(){
    // stredni krabie
    translate([0,0,vyskaKrabice/2])
    difference(){
        cube([stranaKrabice,stranaKrabice,vyskaKrabice],center=true);
        rotate([0,0,45])
        difference(){
            cube(5*stranaKrabice, center = true);
            cube([uhloprickaKrabice,uhloprickaKrabice,5*stranaKrabice],center = true);
        }
    }
    // sikmeVzpery
    vyskaVzperVrohu = 15;
    for(i=[1,-1])for(j=[1,-1])
        hull(){
            translate([i*distanceX/2,j*distanceY/2,vyskaKrabice-vyskaVzperVrohu/2])
                cylinder(h=vyskaVzperVrohu, r=rKrajniSroub+okrajRohu,center = true);
            translate([-i*distanceX/2,-j*distanceY/2,vyskaKrabice-vyskaVzperVrohu/2])
                cylinder(h=vyskaVzperVrohu, r=rKrajniSroub+okrajRohu,center = true);
            translate([0,0,-18])
            cylinder(h=vyskaVzperVrohu, r=rKrajniSroub+okrajRohu);
        }
    //rohy
    for(i=[1,-1])for(j=[1,-1])
        translate([i*distanceX/2,j*distanceY/2,vyskaKrabice+vyskaURohu/2-rohyDolu/2])
            cylinder(h=vyskaURohu+rohyDolu, r=rKrajniSroub+okrajRohu,center = true);

    // stred
    cylinder(h=vyskaKrabice+vyskaStred, r = polomerStred);
    
    /// krytky na hlavicky

    for(i=[1,-1])for(j=[1,-1])
        translate([i*sroubyMotorDist/2,j*sroubyMotorDist/2,vyskaKrabice])
            cylinder(r=rKrytka,h=hHlavicka);
    
    
}

module motor(){
    // motor
    translate([0,0,vyskaMotoru/2])
    difference(){
        cube([stranaMotoru, stranaMotoru,vyskaMotoru],center =true);
        rotate([0,0,45])
        difference(){
            cube(5*stranaMotoru, center = true);
            cube([uhloprickaMotoru,uhloprickaMotoru,5*stranaMotoru],center = true);
        }
    }
    // hridel
    cylinder(h=5*vyskaKrabice, r= polomerHridele);
    // srouby
    for(i=[1,-1])for(j=[1,-1])
        translate([i*sroubyMotorDist/2,j*sroubyMotorDist/2,0])
            cylinder(r=rSroubyMotor, h=5*vyskaKrabice);
    // stredKolo
    translate([0,0,vyskaMotoru])
    cylinder(r=polomerStredKolo, h = vyskaStredKolo);

    // kabel
    translate([stranaMotoru/2, 0,kabelVyska/2])
    cube([stranaMotoru,kabelSirka,kabelVyska], center =true);

}


difference(){
    plna();
    for(i=[1,-1])for(j=[1,-1])
        translate([i*distanceX/2,j*distanceY/2,0])
            cylinder(h=5*vyskaKrabice, r=rKrajniSroub,center = true);
    motor();
    
    for(i=[1,-1])for(j=[1,-1])
        translate([i*sroubyMotorDist/2,j*sroubyMotorDist/2,vyskaKrabice])
            cylinder(r=rHlavicka,h=hHlavicka);

    for(i=[1,-1])for(j=[1,-1])
        translate([i*distanceX/2,j*distanceY/2,-vyskaURohu/2+vyskaKrabice-rohyDolu])
            cylinder(h=vyskaURohu, r=rKrajniSroub+okrajRohu,center = true);
    translate([0,0,-2.5*stranaKrabice])
    cube(5*stranaKrabice, center = true);
}





