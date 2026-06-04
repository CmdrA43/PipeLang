# PipeLang
PipeLang is a compile-time guaranteed high performance language based around ECS design, a pipeline structure, and no runtime allocation of anything at all.
## Main Structure
PipeLang reads like a header file, primarily consisting of definitions and behaviors, with a single `Pipeline` statement at the end with a line of different listed systems and syntax to create a pipeline for the application.
The different built in high-level structures consist of:
* `Component`
* `Entity`
* `System`
* `Pipeline`
### Components
In PipeLang, the first thing you want to define are your components. These will be the things that you attach to different Entity types and also the different arguments you pass to Systems. They are defined as:
```PipeLang
Component Position{ x: f32; y: f32; };
Component Velocity{ dx: f32; dy: f32; };
```
You start with writing out the `Component` keyword, followed by the name of your new component, then list all of its fields inside braces. Fields are defined with the name of the field, followed immediately by a colon, then by a recognized type, and finally, a semicolon.
### Entites
Entites are less of object instances, and more of object classes or structs. They act as a way to create an instance of multiple components all at once, tied to the same identifier. You would write out the definition for one as:
```PipeLang
Entity Particle = { Position, Velocity };
```
They are mildly similar to component definitions, except you obviously use the `Entity` keyword, then your name, and then set it equal to a list of different components you have already defined, enclosed in braces. 
In order to spawn entities, you use `Entity.create(num);` which automatically creates an ID, allocates space for the new components in their respective groups, and then ties those instances to the global ID. You would replace `Entity` with your entity type, and then it would create a number `num` of entites of that type.
