include <BoxCommon.scad>;
$fs=.01;
// This is going to look upside down because of
// coordinate system for the board is from the front.
// Whereas this view is from the back. Its all
// flipped on the x axis.


BlockSZ=[10,20,6+9]; //  Of the board holders
Front_Shift=[0,-CPU_Board_Loc.y-4,0];  // Center of DMM board 
DMM_HolderSZ=[100,23,6+9];

// Locate the clamp elements:
DMM_ClampScrewOffset=(DMM_HolderSZ.x+DMM_BOARD_X)/4;
ClampHole=5;

// Use these modules with the same translate parameters
module DMM_Front_U()
{  

    
   translate([5,-BlockSZ.y/2,BlockSZ.z/2]) cube(BlockSZ,center=true);
   translate([-25,-BlockSZ.y/2,BlockSZ.z/2]) cube(BlockSZ,center=true);
   
   translate([0,-7,DMM_HolderSZ.z/2] + Front_Shift) cube(DMM_HolderSZ,center=true);
}
module DMM_Front_D()
{
    translate(Front_Shift)
    {
       for (bl=boards) 
        {
                   // The board is centered on 0,0 and 7 above z
        #translate(bl+[-DMM_BOARD_X/2,-DMM_BOARD_Y/2,7])

            cube([DMM_BOARD_X, DMM_BOARD_Y,10]);
 
        }
        for (x=DMM_JAX_X)
        {
            translate([x,-DMM_JAX_Y,-.01])
                cylinder(h=20,d=DMM_JAX_LOWER_DIA);
            translate([x,-DMM_JAX_Y,DMM_JAX_Z_OFFS])
                cylinder(h=20,d=DMM_JAX_UPPER_DIA);
        }
        #translate([DMM_ClampScrewOffset,-DMM_JAX_Y,BlockSZ.z/2]) cylinder(h=20,d=ClampHole,center=false);
        #translate([-DMM_ClampScrewOffset,-DMM_JAX_Y,BlockSZ.z/2]) cylinder(h=20,d=ClampHole,center=false);
    }

    
}
*difference()
{
    union(){
        cylinder(r=1,h=50);
        DMM_Front_U();
    }
    DMM_Front_D();
}


Panel2ShoulderZ=44.5;
Panel2BlockTopZ=Panel2ShoulderZ+5;
Panel2TopInsulatorZ=22;
Panel2TopConnectorZ=28;
ConnectorDia=6.6;
BridgeThk=2;


HZ2=Panel2ShoulderZ-DMM_HolderSZ.z;
HZ1=Panel2BlockTopZ-DMM_HolderSZ.z;
HZ4=Panel2TopInsulatorZ-DMM_HolderSZ.z;
HZ3=Panel2TopConnectorZ-DMM_HolderSZ.z;

echo("HZ1-4",HZ1,HZ2,HZ3,HZ4);
echo("HZ1-4+",HZ1+DMM_HolderSZ.z,HZ2+DMM_HolderSZ.z,HZ3+DMM_HolderSZ.z,HZ4+DMM_HolderSZ.z);

BOARD_Neck=53.5;
BOARD_TAIL_X=63.5;


HX=DMM_HolderSZ.x;
HY=DMM_HolderSZ.y;
HZ=HZ1;

boardSlot=[BOARD_Neck,50,30];



module clamp()
{
   difference()
   {
      translate([0,-7,HZ/2] )
      {
          cube([HX,HY,HZ],center=true); // Main body
      }
       // Slot for board neck
      translate([0,3,HZ1/2])
            cube([BOARD_Neck,10,HZ1+0.1],center=true);
      translate([0,3,HZ2/2])
            cube([BOARD_TAIL_X,12,HZ2],center=true);
 // Make a slot for the components on top of the board;.
        translate([0,0,boardSlot.z/2+HZ3+BridgeThk]) cube(boardSlot,center=true);
        for (x=DMM_JAX_X)
        {
            // Lower slots
             translate([x,-DMM_JAX_Y,-.01])
                cylinder(h=HZ4,d=DMM_JAX_UPPER_DIA);
             translate([x-DMM_JAX_UPPER_DIA/2,-DMM_JAX_Y,0])
                 cube([DMM_JAX_UPPER_DIA,20,HZ4]) ;
            // Upper slots
             translate([x,-DMM_JAX_Y,-.01])
                cylinder(h=HZ3,d=ConnectorDia,center=false);
             translate([x-ConnectorDia/2,-DMM_JAX_Y,0])
                 cube([ConnectorDia,20,HZ3],center=false) ;
        }
        
        // The clamp screws
        #translate([DMM_ClampScrewOffset,-DMM_JAX_Y,0])  cylinder(h=HZ1+1,d=ClampHole,center=false);
        #translate([-DMM_ClampScrewOffset,-DMM_JAX_Y,0]) cylinder(h=HZ1+1,d=ClampHole,center=false);

 
   }
}

//color([.8,.8,1,1]) translate([0,0,0]) clamp();
