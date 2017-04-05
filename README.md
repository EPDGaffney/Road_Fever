# Game Plot/Story
Click [here](https://github.com/FortuneMatthew/Road_Fever/blob/master/github%20markdown%20files/public_site_write-up.md) if you wish to read the plot/story behind this game.

# Project Prerequisites
To get the project files, one needs the following things installed:
* [Visual Studio 2015](https://1drv.ms/u/s!AhIK9Eqc9f5mgcglW_t37wRYy7OLNQ)
* [Unreal Engine 4.15.0](https://accounts.unrealengine.com/register)
* Substance Painter for Unreal Engine 4.15.0 (Downloaded via launcher)

# Generating the Project-Files
To generate the project files, one must first have the project cloned to one's desktop. Once one has the project, right click the `Road_Fever.uproject` project file and then click `Generate Visual Studio Project Files`. One must do this every time a class is added or removed from the project [All commits that require a re-generation of the VS project files will have a prefix of RRG (Requires re-generation)].

# Compiling the Project
Once the project files have been generated, open the `Road_Fever.sln` file. This should open in Visual Studio (if it asks what version, it is 2015. Once in the IDE, right click the Road_Fever project within the solution explorer, then set it as the `start-up project`; like so:

![Road Fever startup project](http://image.prntscr.com/image/0674d1cc4ca84ef1ad684617e119b638.png "How to set as startup project")

# Adding to/Editing the Project
Before adding/editing things within the project, make sure you are following this [style guide](https://github.com/Allar/ue4-style-guide#0.1).

# Stuff for Programmers
If you are programming stuff, I have created some snippets for Visual Assist that generate most of the code you need when working with UE4. You can get them from [here](https://github.com/FortuneMatthew/VisualAssistSnippets). I have included installation instructions as well as usage instructions with them. The way these snippets are written is basically the programming-style-guide that we have been using, so it might be worth your while checking them out, even if you haven't Visual Assist.
