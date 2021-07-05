# Homework 1:  Incredible Machines
## Piston
This is a piston like device, to convert a rotation motion into a translation one

### Device definition

#### Parameters:
* `dShaft`: shaft's diameter 
* `stroke`: distance covered by the movement of the piston
* `lRod`: connecting rod's lenght
* `wRod`: connecting rod's width
* `hPiston`: piston's hight
* `dPiston`: piston's diameter
* `angle`: crank's rotation angle 

#### Definizione corpi e vincoli dei parametri:
* __SHAFT__: connection element to the rotation motion, defined by its diameter (`dShaft`).
* __MANOVELLA__: connects the crank to the connecting rod
    - Half the piston stroke (`stroke`) is defined as the crank's lenght. It's the distance between the shaft's axis and crank-connecting rod joint's axis.
    - The crank-connecting rod joint's diameter is equal to the connecting `wRod`.
    - The total crank's lenght depends on: `stroke`, `dShaft`, `wRod`. It's equal their sum plus two addition coupling terms for the joints with the shaft and the connecting rod.
    - The additional term for the connecting rod's joint is 1/5 `wRod`.
    - The additional term for the shaft's joint is 1/5 the `dShaft`.

    CONSTRAINTS:
    - The crank's lenght must be greater the sums of the two joints' radius.
* __CONNECTING ROD__: Connects crank and piston.
    - The connecting rod's lenght (`lRod`) is the distance between the axis of the crank and the piston joint.
    - The piston-connecting rod's joint is inside the piston, with an additional coupling term equal to 1/5 `wRod`.

    CONSTRAINTS:
    - `lRod` must avoid interference between crank and piston in the lower dead point (`angle` equal to 180°). 
    - `wRod` can't be lower than 1/6 `lRod`.
    - `wRod` can't be greater than `dShaft`.
* __PISTON__: connection element to the translation motion.

    CONSTRAINTS:
    - Piston's diameter and lenght (`dPiston` e `hPiston`) must ensure the additional coupling term for the connecting rod joint, so they cant be lower than 7/5 `wRod`.
* __ANGLE__: crank's rotation angle (`angle`) in degrees.

P.S. The coupling term is an additional portion of material around the whole joint edge.

Example image: 

![](device.svg)

### Command line parameters
Command format: `./mainentry -i importPath -e/-eq cxShaft cyShaft exportPath -p {params}`
* `-i` import a device from the file with path `importPath`.
* `-e` export a device on the file with path `exportPath`. The device is taken from:
    - an imported file called with the option `-i` (prioritized action).
    - the one crated with the params passed after the option `-p` (ignoerd if `-i` is called).
    `cxShaft`, `cyShaft` are the coordinates of the shaft's center on the SVG draw.
* `-eq` export a device with quotes on the file with path `exportPath` (options as before).
* `-p` followed by the params of the device to be exported (can't be called if `-e` or `-eq` isn't called before).

    Params: `dShaft stroke lRod wRod hPiston dPiston angle`(defult value 0) (for details see README).

More following params will be ignored 

