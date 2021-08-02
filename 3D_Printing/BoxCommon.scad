



// Sum array members:
function suma(array,start,end) =
    (start==end)
    ? array[start]
    : suma(array, start+1, end) + array[start];
function e(clear,o) =// Add clear to demensions of o
     (is_list(clear)) ?
        [o.x+clear.x,o.y+clear.y,o.z+clear.z] :
        [o.x+clear,o.y+clear,o.z+clear];

function align(mode,here,center=true)=
    [ search(mode,"eErR")?here.x:(search(mode,"wWlL")?0:here.x/2),
    search(mode,"nN")?here.y:(search(mode,"sS")?0:here.y/2),
    search(mode,"tT")?here.z:(search(mode,"bB")?0:here.z/2)]-
    (center?[here.x/2,here.y/2, here.z/2]:[0,0,0]);


module cubeXY(cc)
{
    translate([-cc.x/2,-cc.y/2, 0]) cube(cc,center=false);
}
 
DISPBLOCK=[16+50.2*3,44,15]; // Z is arbitrary at the moment

// FF=Fudge Factor

DMM_BOARD_X=66 ; // 63 NO FF FROM DRAWING
DMM_BOARD_Y=3 ; // 1.6 NO FF Measured
//DMM_Center=[50,-20,0];

// Extra board locations relative to the DMM board:
CPU_Board_Loc=[-10,27,6];  // Not using full width of the board
// loc is offset left to miss drilling the screw block.
IF_Board_Loc=[0,15,6];
boards=[[0,0,0],IF_Board_Loc,CPU_Board_Loc];


S_VO_OFFS=-19.08; // From drawing
S_Widths=14; // FROM Drawing
// Places 5A jack at edge of board.:
S_FUDGE=DMM_BOARD_X/2-S_Widths*2-2; 
S1=-S_Widths/2+S_FUDGE;
S0=S_VO_OFFS+S1;
S2=S_Widths/2+S_FUDGE; 
S3=S2+S_Widths;
DMM_JAX_X=[S0,S1,S2,S3];

// STack up to C/L of jack in Y - Measured terms
DMM_JAX_Y=DMM_BOARD_Y/2+4.9+5.5/2; 
DMM_JAX_LOWER_DIA=12; // Measured
DMM_JAX_UPPER_DIA=15; // Same as S_Widths
DMM_JAX_Z_OFFS=3.3 ; // How much of the lower shoulder shows.

for (i=[DMM_JAX_X])
    echo(i);
echo(DMM_JAX_Y);


