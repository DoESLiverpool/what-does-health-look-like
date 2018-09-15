var x = 1
var y = 1

var serial
var inData
var sensorInput

function setup() {
  createCanvas(windowWidth, windowHeight, SVG);
  background(100);
  }

function draw() {
    print()
    noStroke();
	if (frameCount > 150){
        noLoop();
		save();
    } else {
        ellipse(400+100*cos(2*x)*cos(x), 300+100*cos(2*x)*sin(mouseX), 50, 50);
        x+=0.05
        y+=.05
}
}

