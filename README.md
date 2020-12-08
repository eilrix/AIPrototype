# AIPrototype
For demonstration just build the project and hit "Play" on startup map

Order of execution:
* __AAIPrototypeGameMode__ spawns AI players (__AAIPlayerController__) on StartPlay
* AI players has __UUnitsControlComponent__ on it that spawn's __AUnitSpawner__
* Unit spawner spawns unit group along with it's commander that is possesed by __AUnitGroupAIController__
* AUnitGroupAIController uses Behavior Tree to control group behavior
