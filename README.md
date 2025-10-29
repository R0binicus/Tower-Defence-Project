# Tower Defence Project

A automated turret project that I created in the Unreal Engine, made with C++, as part of my course at CG Spectrum.

For this project, I had to make a turret that tracked and shot at enemies for a tower defence game. For this, I was given a Unreal Engine 3rd-person project that included a basic level and unlike the previous minimap project, it didn’t include the rest of the game. So my first challenge was figuring out how to test the turret shooting and tracking, without spending too much time working on implementing the enemies. The solution I came up with was to import the player robot from the minimap project and add a health interface onto it. This allowed me to simulate multiple enemies and movement without needing to create a spawning system or enemy AI. 

My next task was to implement target tracking. The first step was to find the closest enemy’s location, which was easy. However, I then had to figure out how to get the correct direction the turret needs to face, then rotate towards that angle. To do this, I got the dot product of the turret’s facing direction and direction to the enemy. I then calculated the dot product’s arccosine to get the degrees to the enemy. The only issue being that this doesn’t say which way the turret needs to rotate. I solved this by using the cross product of the turret facing direction and the direction to the enemy, then getting the sign, which gave me the anti/clockwise direction (signed angle). I could then use a linear interpolation to make the turret rotate over time to face the enemy, and fire a projectile when the turret was facing the enemy, again using the dot product.

By this point, I had already done all I needed for the brief, so I had time for extension work! So, I considered what else turret related I could implement that might be useful for a tower defence game. What I settled upon was adding different turret aiming modes and projectile types. 
The projectile types were fairly simple, the original, base projectile just used Unreal physics with an initial launch velocity. I then added a homing projectile that used a lerp to go towards the enemy and finally a ‘smart homing’, which gradually increases its homing as it gets closer to its target. 

The next step was the turret aiming modes, which is where the main difficulty was started. The base aiming mode just faces the enemy, shoots and often misses as it ignores gravity. I wanted to make the turret consider gravity when changing its pitch to aim at the enemy target, shooting the projectile in an arc. I knew this was a projectile motion problem, which I learnt all about in high school, however it's been a while and I was a bit rusty. Luckily I found a blog post of someone solving this exact problem, but when I implemented it, the equation didn’t work. After spending close to a day on this problem, I remembered that in high school we always needed to make sure our calculators were in degrees, not radians. After a quick check, I confirmed that, yup, trigonometric functions in C++ expect the input to be in radians, not degrees. After that, it worked great!
Blog post mentioned can be found here: https://www.forrestthewoods.com/blog/solving_ballistic_trajectories/ 

This is not an exhaustive list of what the turret includes, just some highlights. I added several other features, such as a simple prediction of an enemy’s future location, using its velocity.

If I had the opportunity to do this again, I would probably try to add more turret features, such as a laser beam turret, improved projectile smart homing, and change the existing projectile and turret features to be more data-driven. I also might make a generic object pooling component with interfaces, to allow me to more easily reuse object pooling for more circumstances. This is already the second time I’ve used it for this course! 

My main focus for this was getting better at and practicing general game programming skills and Unreal C++. I’ve definitely got a lot out of these past three weeks, had a great time and learnt a lot.

## Key Features

- **Turret Tracking System**: Automated turret tracks and turns to face the nearest enemy.
- **Turret Aiming Modes**: Turret can be set to aim directly towards the enemy, at an angle so that the projectile moves in an arc, or with future position prediction.
- **Projectile Behaviour Types**: Projectiles can be launched with initial velocity, with homing, or a combination of the two.
