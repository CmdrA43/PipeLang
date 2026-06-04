# PipeLang
PipeLang is a compile-time guaranteed high performance language based around ECS design, a pipeline structure, and no runtime allocation of anything at all.
## Main Structure
PipeLang reads like a header file, primarily consisting of definitions and behaviors, with a single `Pipeline` statement at the end with a line of different listed systems and syntax to create a pipeline for the application. The different built in high-level structures consist of:
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
In order to spawn entities, you use `Entity.create(num);` which automatically creates an ID, allocates space for the new components in their respective groups, and then ties those instances to the global ID. You would replace `Entity` with your entity type, and then it would create a number `num` of entites of that type. However, nothing is actually created. You define a number for your max number of entities at the top of the file, and then it sets an alive/dead flag for the data. Everything in memory is actually stored in an SoA or sparse set style ECS.
### Systems
Systems are not necesarily functions in the classic sense. When you define a system, you write something that looks like it will run once, and then the compiler makes sure to generate code that iterates it over all entities with the given inputs. A system definition looks something like this:
```PipeLang
System integrate(edit Position, read Velocity){
  let dt: f32 = 0.016;
  Position.x += Velocity.dx * dt;
  Position.y += Velocity.dy * dt;
};
```
  When defining a system, you start with the `System` keyword, then put the name you want it to have, then enclose all the required components in parentheses. Note the words before the different referenced components, namely `edit` and `read`. These are special access identifiers used to determine how the system accesses the data, and whether that data should be mutable by that system or not. The three access keywords are:
* `read` (read only permissions)
* `write` (write only permissions)
* `edit` (both read and write permissions)

This field list tells the compiler how to properly structure the function so that it can correctly identify all user-defined entities that it should pass to the function.  
After this are some braces, which contain the function body. Here, the Position component needs `edit` privileges, because it both has to read the data (to know what to increment from) and write to it. However, the Velocity component only needs `read` privileges, because it is only used to increment the velocity.  
For systems where you don't need to iterate over any entities, you simply don't give it any arguments.  
The next part of systems is the `Threaded` keyword. This tells the compiler to construct the final application in a manner that allows that system to be dispatched across multiple execution threads if needed. If we wanted to take our `integrate` system and make it threaded, we would simply change the first line to look like this.
```Pipelang
System Threaded integrate(edit Position, read Velocity){...};
```
### Pipelines
At the very end of everything, you have to tell the compiler how you want it all to come together. This is where pipelines come in. They take all of the keywords thrown in, put a couple extra ones on the front and back for things like I/O, and then compile it. A pipeline is usually defined like this:
```PipeLang
Pipeline
  { input_handler | AI_update } >> physics >> { animation | sound } >> render;
```
When writing a pipeline, you can group, `{}`, fuse, `|`, and seperate, `>>`, different systems into stages. Systems can be fused to run at the same time, basically acting as the same system, as long as they don't have any colliding `edit` or `write` components. Another more complicated example could look like:
```Pipelang
Pipeline
  {{A >> B} | C} >> D >> {E | F} >> G ;
```
## Future Feature List
* Finishing the parser
* Expression lexing and parsing
* Full Pipeline feature integration
  * `>>` for stage seperation
  * `{}` for grouping of stages
  * `|` for stage fusion
* Compile Time performance guarantees
  * WCET calculation for each system
  * In-depth memory usage analysis
