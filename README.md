# Ubisoft develop program 2023
![c++](https://img.shields.io/badge/C++-Solutions-blue.svg?style=flat&logo=c%2B%2B) ![Figma](https://img.shields.io/badge/figma-%23F24E1E.svg?&logo=figma&logoColor=white)

# "City Runner" -  Project Documentation
Welcome to the documentation for the "City Runner" project, an Infinite Scroller game developed as part of the Ubisoft Develop Mentorship program. In this documentation, you will find a detailed account of the design, development, and technical aspects of the game.
<p align="center">
<img  src="https://i.imgur.com/HFEgqpG.png" alt="gameplay screenshot1" width="700"/>
</p>

## Project Overview

The "City Runner" project represents a technical challenge aimed at showcasing programming skills and challenging myself in the domain of game development. As a participant in the Ubisoft Develop Mentorship program, the task was to create an Infinite Scroller game using modern C++. The challenge extended beyond mere gameplay, emphasizing the importance of clear code structure, reusability principles, efficient memory management, ownership principles and the strategic use of modern C++ concepts.

# Game overview
***
"City Runner" takes place in an urban environment where players must navigate obstacles, defeat enemies, with the primary objective of collecting money and surviving as long as possible. The game is dynamically generated with random creation of city platforms and landscapes, ensuring a diverse and intriguing gaming experience. Furthermore, various pickups and power-ups are available, which will be detailed further in the subsequent sections of this document.

### Main character
The main character in this game is a young punk navigating through the urban landscape, weaving through obstacles and adversaries with agility. With a rebellious flair, the character races through the city streets, aiming to collect as many money as possible.  
The main character possesses a set of four distinct actions:
* ![run](https://imgur.com/ZdMIRsw.png) **Run:** Default
* ![jump](https://imgur.com/xIIthAM.png) **Jump:** Activated by the up arrow key
* ![doubleJump](https://imgur.com/a3tdY2v.png) **Double Jump:** Executed by pressing the up arrow key after an initial jump
* ![attack](https://imgur.com/IpZlmw2.png) **Attack:** Executed by pressing the spacebar to combat enemies

### Enemies
The enemies in this game are adversaries that introduce an element of challenge to the gameplay. Each of them has a single life and moves back and forth along a game platforms. Upon detecting the player's presence they will speed up and once in close proximity, they unleash a melee attack with their mace, inflicting damage and deducting one life from the player.
<p align="center">
  <img  src="https://imgur.com/qFUg7r4.png" alt="enemy"/>
</p>

### Collectibles and Power up
The game contains various pickable items that enhance the gaming experience:

* ![money](https://imgur.com/fAhd4Q7.png) **Money:** Collecting money adds to your final score. The more money you collect, the better you've played.
* ![speedBost](https://imgur.com/9Mdig71.png) **Speed Boost:** Increases the player's speed by 50% for 5 seconds
* ![jumpBoost](https://imgur.com/h3HmnJd.png) **Jump Boost:** Enhances the player's jump height by 30% for 5 seconds
* ![healthUp](https://imgur.com/GFJ9DDA.png) **Life Boost:** Adds an extra life to the player

### Game environment
The game environment provides a dynamic experience with its randomly generated world. The floor platforms, forming the foundation for the character's movement, vary in height while maintaining a maximum distance of 3 steps from each other. This arrangement creates an ever-changing and engaging gameplay terrain. Each floor platform is capped by upper platforms, distributed across two levels, introducing additional variety and complexity to the game path.

The upper platforms, with their variable sizes and positions, offer players multiple tactical and strategic options during their run. Importantly, enemies do not appear on the upper platforms, allowing players to focus on evolving challenges related to the terrain below. Amidst the floor platforms, players may encounter a range of elements, including pickables, power-ups, obstacles, and enemies.

<p align="center">
    <img  src="https://imgur.com/uH8Q3Yj.png" alt="platforms"/>
</p>

### Health System
The player starts with 3 lives and can accumulate up to 5 lives. Lives are gained through the Life Boost power-up, and they are lost by colliding with obstacle barrels or being hit by an enemy. Losing a life grants the player a 5-second invulnerability period. Obstacles are represented by red barrels of various types: ![barrel](https://imgur.com/q1VeaiJ.png) ![barrel](https://imgur.com/7YvQGot.png) ![barrel](https://imgur.com/bvDqCNc.png)

### HUD
<p align="center">
      <img  src="https://imgur.com/LxUojMX.png" alt="HUD"/>
</p>
The HUD is designed to be simple and intuitive. On the left side, the player's score is displayed (representing the amount of money collected). The right side indicates the current number of lives the player has. The center of the UI is dedicated to the player's status: *(in order from left to right)* invulnearability status, speed boost status and jump boost status.

## Game Controls
<p align="center">
      <img  src="https://imgur.com/vFrFiE5.png" alt="controls"/>
</p>

# Technical Overview
***
## Key Features of the Architecture:
* **Entity-Driven Architecture**
This project adopts an entity-driven design philosophy, where the fundamental building blocks are entities that encapsulate specific functionalities. Each entity serves as a cohesive unit, embodying a unique set of behaviors through the incorporation of modular components. These entities, whether representing game objects or system elements, are distinctly identified by a unique identifier, facilitating seamless communication and management within the system.

* **Component Pattern**
The project leverages a modular component pattern to define and organize entity behaviors. In this design paradigm, functionalities are decomposed into independent and reusable components. Each component encapsulates a specific aspect of an entity's behavior. This approach not only promotes code reusability but also simplifies the addition, removal, or modification of functionalities, contributing to overall system scalability and adaptability.

* **Flexible Event Management System**
In addition to the modular architecture, City Runner's incorporates a flexible event management system. This system is based on various event types, each represented by individual data structs. The concept of event management will be explored further in the subsequent chapters, providing a comprehensive understanding of its implementation.

## Entities Life Cycle

```
    void Init();
    void OnBeginPlay();
    void Update(float deltaTime);
	void OnBeginDestroy();
	void DeInit();
```

The journey of a newly instantiated instance, whether it be an object or a component, unfolds through **five distinct phases**. The first stage is the initialization phase, marked by the execution of the **Init** method. This initial setup allows the entity to configure itself and prepare for subsequent interactions within the system. Following initialization, the entity enters the **On begin play** phase, where it actively engages in the system's processes. This stage serves as an opportune moment for executing any setup-related callbacks or additional actions associated with the beginning of the entity's active participation.

As the entity continues its existence, it undergoes periodic updates through the **Update** phase. This dynamic stage allows the entity to adapt and respond to real-time changes, ensuring its behavior remains synchronized with the evolving state of the system.

The life cycle then transitions to the **On begin destroy** phase, initiated by the OnBeginDestroy method. This phase signals the impending destruction of the entity and provides developers with an opportunity to perform any necessary cleanup or pre-destruction actions. Lastly, the life cycle concludes with the deinitialization phase, executed by the **DeInit** method. In this phase, any remaining resources associated with the entity are released, ensuring a graceful and efficient exit.

## Game Loop
When the program is launched, a *Game Instance* entity is instantiated, marking the commencement of the game loop. It serves as an efficient controller, overseeing various *Game Modes* for specific phases: main menu, gameplay, and game over. Deliberately designed for simplicity, this choice guarantees seamless transitions and maintains a clear, manageable codebase. This entity always has a reference to the current Game Mode.

The game architecture revolves around two fundamental types of Game Modes, each playing a distinct role in shaping the player's experience. Firstly, the *Menu Game modes* are tailored to showcase static UI pages represented by UIComponents, guiding users through the interface and setting the stage for the upcoming gameplay. Secondly, the *Gameplay Game Mode* operates as a robust manager, efficiently handling all sub-managers and objects within the game scene.

<p align="center">
      <img  src="https://imgur.com/NtsSN3P.png" alt="gameModes" width="500"/>
</p>

## Core Components
A component is a self-contained and reusable module that manages specific behaviors and functionality of an object. They are typically initialized at the object owner's initialization, and their lifecycle is handled by the owner itself. Usually, they are deinitialized and then destroyed upon the owner's deinitialization.
The core gameplay components of the game are:
* **Transform Component:** Handles the transformation attributes of each game object, including its position, rotation, and scale.
* **Camera Component:** Manages the position, rotation, and zoom of the in-game camera, providing a dynamic and responsive view of the game world.
* **Collision Component:** Handles collision detection and response, allowing game objects to interact with each other and the environment based on defined collision rules.
* **Mesh Component:** Represents the visual geometry or model of game objects, facilitating the rendering and visual representation of in-game entities, using the provided CSimpleSprite implementation.
* **Movement Component:** Governs the movement and navigation of game actors, implementing a state machine-like algorithm for transitioning between running, jumping, falling and more.
* **Animation Component:** Controls the animation state and playback for characters or objects.
* **Input Component:** Manages user input, capturing and interpreting player commands to trigger specific actions or behaviors within the game.
* **Attack Component:** Empowers game entities with the ability to perform various attack actions, allowing them to engage and defeat enemies.
* **Sound Component:** Manages the playback of audio, providing aural feedback for in-game actions and events.
* **Perception Component:** Endows non-playable entities, such as enemies, with environmental awareness, enabling them to recognize the player, navigate through platforms, and make informed decisions based on their surroundings.

## Event Management
The **GameEventManager** is a versatile event management system designed to facilitate communication and coordination between different components. This event manager employs a **template-based approach**, allowing developers to subscribe and publish events of various types seamlessly. Subscriptions are organized by a unique identifier, ensuring clear differentiation between event channels. The *Subscribe* method enables the registration of event handlers for specific event types, associating them with a unique identifier. When events are published using the *Publish* method, the event manager efficiently dispatches them to the corresponding subscribers based on both the unique identifier and the event type. The *RemoveAllSubscriptions* method allows the removal of all subscriptions associated with a particular unique identifier, providing a convenient way to clean up resources. Additionally, the *RemoveSubscription* method allows the selective removal of a specific event handler for a given event type and unique identifier. The implementation leverages standard C++ features, including type indices, function pointers, and data structures like unordered maps and vectors, ensuring flexibility and efficiency in event handling.

```
class CGameEventManager {
public:
    using EventHandler = std::function<void(FGameEvent*)>;

    template <typename EventType>
    void Subscribe(const int uniqueId, const std::function<void(EventType*)>& handler) {
        ...
    }

    template <typename EventType>
    void Publish(const int uniqueId, FGameEvent* eventData) {
        ...
    }

    void RemoveAllSubscriptions(const int uniqueId) {
        ...
    }

    template <typename EventType>
    void RemoveSubscription(const int uniqueId, const std::function<void(EventType*)>& handler) {
        ...
    }
    
private:
    std::unordered_map<int, std::map<std::type_index, std::vector<EventHandler>>> m_subscribers;
};
```

## Collision system
The collision system in this game is orchestrated by the *CollisionManager* and *CollisionComponent* classes, working in tandem to provide robust collision detection and response. The manager is responsible for handling collision presets, managing the registration and unregistration of collision components, and overseeing the collision resolution process.

Upon initialization, the **Collision Manager** configures collision presets, defining how different collision profiles interact. These profiles encompass entities such as characters, enemies, obstacles, collectibles, floors, and more. During runtime, **Collision Components** register with the manager, allowing it to efficiently track interactions between game entities.

The heart of the collision system lies in the Update method of the manager, where it iterates through *"dirty"* registered components, checks for potential collisions, and invokes collision responses. The *CheckCollision* function within the manager compares bounding boxes and, upon collision detection, triggers the appropriate response based on the collision presets.

The collision responses include *blocking collisions*, where entities are prevented from overlapping, and *overlapping collisions*, signaling an event when entities intersect. The system further handles the initiation and conclusion of collisions, notifying relevant components through the event manager.

It's important to note that the Collision Components utilize rectangular bounding boxes for collision detection, offering a simplified and efficient approach similar to the AABB method.

<p align="center">
      <img  src="https://imgur.com/Ip2s3kF.png" alt="collision" width="700"/>
</p>

## Game Aura system
In-game resources such as coins, health, speed, jump height and invulnerability are managed by a single system: the Game Aura System. The *GameAuraComponent* and *GameAura* classes collectively form a comprehensive system for managing game auras and their impact on resources.
* The **GameAuraComponent** class is designed to be attached to game entities, providing them with the ability to have resources and manage auras. It maintains resource values, both current and maximum. The component can activate, deactivate, and update auras during the game, ensuring that their effects are applied appropriately.
* The **GameAura** class represents individual auras objects with distinct properties. Auras can have different damage modes (flat, current amount percentage, or maximum amount percentage), associated resources, effects, and duration. The class implements methods for starting, updating, and ending auras, applying their effects to the associated Game Aura Component. 

 Actors can have multiple auras active simultaneously, and the system ensures that the effects are applied and updated correctly, contributing to a dynamic and engaging gameplay experience.

## UI system
The UI system follows a composition-based approach where **UIComponent** manages multiple **UIElement** instances. This modular design allows for the addition and removal of UI elements dynamically. The system considers camera movement and adjusts UI element positions accordingly.UI elements are designed maintaining the principles of Single Responsibility, managing one graphical entity at a time, such as sprites handled by the **UIElementSprite** or text by the **UIElementText**.

## World management and object pooling
The architecture of the game is based on a world management system facilitated by specialized managers. These managers are pivotal in orchestrating the lifecycles of specific entities.

At the core of this managerial framework stands the *World Manager*, a central entity entrusted with overseeing the entire game world. This managerial maestro takes on the responsibility of coordinating various subsystems, each managed by specialized managers catering to specific aspects of the game. It particularly focus on game platforms, from their initialization to their destruction.

In parallel, this manager takes care of updating specialized managers dedicated to specific entity types. The *Enemy Manager*, for instance, orchestrates the life cycle of in-game enemies, ensuring dynamic and engaging encounters. Simultaneously, the *Collectible Manager* takes charge of pickups and power-ups, carefully managing their lifecycles for player interaction.

Crucially, their implementation incorporates an object pooling mechanism. This system optimizes resource utilization and significantly enhances runtime performance by handling the instantiation and reclamation of game objects.
Furthermore, the *Background Pool* takes charge of background elements, utilizing a pooling mechanism for background sprites. 

# Final Notes
This repository includes the code produced by Federica Bucchieri during the mentorship program. Please note that it does not contain all the necessary files to run the project, as certain components are proprietary to Ubisoft Milan and cannot be published. However, you can still experience the game by running the provided GameTest.exe.
