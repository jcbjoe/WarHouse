# Warhouse Project Github

## Getting Started

### Prerequisites
- Unreal Engine 4.26.0
- Visual Studio 2017/2019
- A Git Client - (GitKraken/Github Desktop)

### Building Project Files (First Install)

This is required when the repository is first downloaded/clones locally. In the project folder you will see a file called: "Warhouse.uproject". Right click on this and select "Generate Visual Studio project files"

![](https://i.gyazo.com/51159015bd3b949ff98fcbd45845abd4.png)

This will generate the project files.

You can then launch Unreal Engine 2 ways.

1. Double click the Warhouse.uproject
2. Double click Warhouse.sln and run Unreal Engine from Visual Studio (Play Button)

### Unreal Engine

The Unreal Engine version that has been used to create the project is: **4.26.0**
This will be the version everyone should be using. If a new version comes out and has noticable benefits we can upgrade, but for now stick to the above version.

### Visual Studio
Im using Visual Studio **2019** - Version is personal preference so thats up to you. To make intelisense faster there is a simple trick that can be done.

Inside Visual Studio, 
- Go to Project
- "Project Name" Properties
- NMake on the left sidebar
- Put /Yu in the Additional Options under IntelliSense

Source: https://forums.unrealengine.com/development-discussion/c-gameplay-programming/65252-for-those-who-suffer-from-visual-studio-intellisense-slowness

Can confirm it helps quite a bit

## Git Flow

Git flow will be used for this project. This is a standard which some people/companies use for a better Git workflow.

Here is a image that explains it fairly well.

![](https://i.gyazo.com/5e2b0ac473ed6ea7282bb1188dcfea43.png)

If using GitKraken this can be enabled by:
- Going to File (Top Left)
- Preferences
- Gitflow
- Initialise Gitflow

This will enable the following tool:
![](https://i.gyazo.com/a673146e79159d9f34dd543537501453.png)

As seen in the image, feature branches can be created and finished here.

### Develop Branch
This branch is the main development branch, commits don't usually go in here. Mainly what this branch is for is a unstable version of the project. Feature branches get merged into here. Only bug fixes are usually commited straight onto develop

### Feature Branch
These branches are for tasks - in our case tasks that are on trello. Having tasks seperated means that we dont overwrite each others work. The naming convention for feature branches is feature/(TaskName). If you are using GitKraken and have the Trello integration setup you can create feature branches by right clicking cards.

![](https://i.gyazo.com/3a8e676c06f80d21f002c4e05ea922fa.png)

Once a feature branch a pull request can be made or it can be simply merged in. Pull requests are useful if you want someone else to look at your work.

### Release Branch
We can ignore these for now, but are usually used when getting read to deploy a release and this branch goes to QA testing (In real world)

### Master Branch
This is where when a sprint is complete the develop branch will be merged. Its a mostly stable version of the code.

## Folder Structure

Repository structure is fixed, and it only has a few toplevel directories. Every other directory or file is ignored.

- `/Source`
- `/Config`
- `/Plugins`
- `/Content`
- `/RawContent`

`git-lfs` management rules are mostly defined for file types, and not _paths_, however there can entire paths marked to be managed by `git-lfs`. Without a special note, expect only type-based rules apply to a directory.

#### `/Source`

C++ source code is stored under the `/Source` path. As with most other directories, this directory is managed by standard git (and not `git-lfs`). That means no blobs. Do not put here any `.dll`s, `.exe`s, `.zip`s and other binaries. Only text files are allowed.
Generated text files can reside in the local `/Source` dir, but should be ignored by git with additional entries in `.gitignore`.

#### `/Config`

Engine and game config files.

#### `/Plugins`

Game plugins. Every plugin lives in a subdirectory of the `/Plugins` dir. A plugin internal directory structure is not strictly documented, so there are no assumptions on how a plugin is structured.
It may be useful to use git submodules to manage plugins in a more robust manner.
It is expected that each plugin will have it's own `.gitignore` file in it's subdirectory, as well other required specific git tweaks.

#### `/Content`

Game assets in Unreal Engine formats, `.uasset` and `.umap`. Only those two file types are allowed, everything else is ignored.

#### `/RawContent`

**This directory is managed entirely by `git-lfs`.**

`/RawContent` is a directory where you store assets in their source formats, in contrast to `/Content`, where assets are stored in the engine format (after the import). Having an asset in a source format is useful when you're still making updates to it. It may be a good idea to also have separate repos for managing work-in-progress assets (maybe in smaller collections or even idividually).

Copied from: https://github.com/MOZGIII/ue4-gitignore