console.log("Hello, World!");

const Engine = Matter.Engine,
    Render = Matter.Render,
    Runner = Matter.Runner,
    Bodies = Matter.Bodies,
    Common = Matter.Common,
    Mouse = Matter.Mouse,
    MouseConstraint = Matter.MouseConstraint,
    Composite = Matter.Composite,
    Composites = Matter.Composites;

//Create engine
let engine = Engine.create(),
    world = engine.world;

//Create renderer
let render = Render.create({
    element: document.getElementById("skills"),
    engine: engine,
    options: {
        width: 800,
        height: 600,
        showAngleIndicator: false,
        wireframes: false
    }
});

//Create mouse
let mouse = Mouse.create(render.canvas),
    mouseConstraint = MouseConstraint.create(engine,
        {
            mouse: mouse,
            constraint: { stiffness: 0.2, render: { visible: false } }
        }
    );

//Sync mouse with renderer
render.mouse = mouse;

const skills = ['CXX', 'C', 'CSHARP', 'PYTHON', 'HTML', 'CSS', 'JS'];

//Create ground and walls
let ground = Bodies.rectangle(400, 610, 800, 60, { isStatic: true });
let wallLeft = Bodies.rectangle(0, 610, 60, 800, { isStatic: true });
let wallRight = Bodies.rectangle(800, 610, 60, 800, { isStatic: true });

//Add bodies to Composite
let stack = Composites.stack(100, 0, 10, 8, 10, 10, function(x, y) {
    const radius = Common.random(15, 30)
    const skill = skills[Math.floor(Common.random(0, skills.length))];
    const texture = './imgs/skills/' + skill + '.png'

    let body;
    if (skill === 'JS'){
        body = Bodies.rectangle(x, y, radius*2, radius*2, { restitution: 0.6, friction: 0.1, render: { sprite: { texture: texture } } } );
    } else {
        body = Bodies.circle(x, y, radius, { restitution: 0.6, friction: 0.1, render: { sprite: { texture: texture } } } );
    }

    //Correct texture size
    let img = new Image();
    img.src = texture;
    img.onload = function() {
        body.render.sprite.xScale = (radius * 2) / img.width;
        body.render.sprite.yScale = (radius * 2) / img.width;
    };

    return body;
});

Composite.add(world, [ground, wallLeft, wallRight, stack]);
Composite.add(world, mouseConstraint);

//Run the renderer
Render.run(render);

//Create runner
let runner = Runner.create();

//Run the engine
Runner.run(runner, engine);