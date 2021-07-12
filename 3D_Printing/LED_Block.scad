include <BoxCommon.scad>;
       
DIS54=[50.7,22,7.8]; // Alphanumeric
DIS56=[50.7,20,8.0]; // Digits
FEATHER=[50.7,23,15];
E_FEATHER=e([4,0,0],FEATHER);
offsx=FEATHER.x;
notch=[10,4,2];
npos=14.5;

difference()
{
    // Main block
    translate([0,0,DISPBLOCK.z/2]) cube(DISPBLOCK,true);
    // Cutouts:
    // This is the LED Block
    translate([offsx,0,DIS54.z/2-.01]) // Lower part
        cube(e([1,0,1],DIS54),true);
    // This is the slot the feather drops into
    translate([offsx,0,DIS54.z+FEATHER.z/2]) //upper part
        union()
        {
            cube(E_FEATHER,true);
            // Notches for the soldered terminals.
            translate([npos,notch.y/2-FEATHER.y/2,-FEATHER.z/2])
                cube(notch,true);
            translate([-npos,-notch.y/2+FEATHER.y/2,-FEATHER.z/2])
                cube(notch,true);
        }
    
    translate([0,0,DIS56.z/2-.01])
        cube(e([1,0,1],DIS56),true);
     translate([0,0,DIS56.z+FEATHER.z/2]) 
         union()
        {
            cube(E_FEATHER,true);
            translate([npos,notch.y/2-FEATHER.y/2,-FEATHER.z/2])
                cube(notch,true);
            translate([-npos,-notch.y/2+FEATHER.y/2,-FEATHER.z/2])
                cube(notch,true);
        }
            
    translate([-offsx,0,DIS56.z/2-.01])
        cube(e([1,0,1],DIS56),true);
     translate([-offsx,0,DIS56.z+FEATHER.z/2]) 
        union()
        {
            cube(E_FEATHER,true);
            translate([npos,notch.y/2-FEATHER.y/2,-FEATHER.z/2])
                cube(notch,true);
            translate([-npos,-notch.y/2+FEATHER.y/2,-FEATHER.z/2])
                cube(notch,true);
           }
        // Corner marker:
          translate([DISPBLOCK.x/2,-DISPBLOCK.y/2,DISPBLOCK.z/2])
                rotate([0,0,45]) cube([5,5,DISPBLOCK.z+5],true);
        }
    