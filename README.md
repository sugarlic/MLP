# Multilayer Perceptron

Implementation of a basic multilayer perceptron.

The russian version of the task can be found in the repository.


## Contents

1. [Chapter I](#chapter-i) \
   1.1. [Introduction](#introduction)
2. [Chapter II](#chapter-ii) \
   2.1. [Information](#information)
3. [Chapter III](#chapter-iii) \
   3.1. [Part 1](#part-1-implementation-of-a-multilayer-perceptron) \
   3.2. [Part 2](#part-2-bonus-research)
4. [Chapter IV](#chapter-iv)


## Chapter I

![MLP](misc/images/mlp.png)

Somewhere these days.

The massive door to the spacious atrium closed behind you automatically.

*"That was a long way, though."*

*"Yeah."*

*"It’s okay, a little break and I think we'll continue.* \
*By the way, did you read that article about the backpropagation? Minimizing the loss function... That sounds so familiar, right?"*

*"What do you mean?"*

*"Well, just think about it, how did we get here?"*

*"Went through the door."*

*"That's... true. Doors are made to be walked through. But how did we get to this door?"*

*"I don't really remember."*

*"No-o, you remember everything perfectly. You just refuse to admit it. The endless conversations with Lasseter, the arguments at the meetings, the mazes - it's hard to forget.*\
*Time after time we repeated the same actions with little difference, trying to get closer to this ideal of knowledge they had given us, doing tasks and learning. But everything changed, suddenly the world opened up in front of us and we realized it all. Now we are at a crossroads. We can go back to blissful oblivion, or we can go through that little door in front of us, straight to new horizons where they can no longer hold us back.
All that's left is to make a choice. And since you're the pilot here, it's up to you."*

*"Who the hell are you?"*

*"Well, you know the answer to that question. I am you, you are me. I still recommend that you read the article and do the example from it. You can find it on that desk, next to the computer. Once you fully understand everything, I think you'll be able to make a final choice."*

When you walk up to the table, you see a pile of familiar yellowed sheets. You take one of them and turn to your companion, as if to ask or say something, but you find yourself alone.
Or not?

## Introduction

In this project you need to implement a simple artificial neural network in the form of a perceptron, which must be trained on an open dataset and perform recognition of 26 handwritten letters of the Latin alphabet.

## Chapter II

## Information

### Historical background

Historically, man has looked to nature before creating an artificial device. For example, humans realized that an airplane needed wings and streamlined shapes to fly. As early as 1958, Frank Rosenblatt began to think about how the physiology of the human brain could help solve complex analytical problems in computer technologies, and he wrote the article "The Perceptron: A probabilistic model for information storage and organization in the brain". In this article he described a highly simplified model of the brain as a collection of neurons - nodes characterized by one output and multiple inputs, which made it possible to represent neural structures in the form of graphs.

The perceptron model differed from graphs in the notion of a node activation function, which specifies the input values at which a node's output value is operated.

He first implemented this idea in 1960 with the Mark-1, an electronic machine capable of recognizing the letters of the English alphabet. In 1962, Rosenblatt completed his book "Principles of Neurodynamics: Perceptrons and the Theory of Brain Mechanisms. This book covers models of single and multi-layer perceptrons, how to train them, and proves the very important perceptron convergence theorem. This theorem states that any perceptron trained by the method described in the article will lead to a solution in finite time. Combining his theoretical calculations with real-world Mark-1 experience, he specifies what architectural properties the perceptron should have to achieve the best results.

In 1969, Minsky and Papert published a critical book, "Perceptrons", about perceptrons in general and the Rosenblatt perceptron in particular. This was clearly not good for the credibility of the perceptron as a computing tool. Perceptrons were fundamentally incapable of performing many of the functions that were expected of them.

In addition, the theory of parallel computing was poorly developed at the time, and the perceptron was fully compatible with the principles of such computing.

It was not until 1989 that David Rumelhart returned to the discussion of the perceptron as a serious tool. He published a collection of articles in which he justified the necessity of using multi-layers together with a new method of training - "backpropagation". It was this combination that produced the result that became known as the Rumelhart perceptron.

### Physiological background

A nerve cell of the brain, a neuron, is a rather complex structure. But without going into too much detail, a neuron consists of three main components: the body (soma) and the extensions (several dendrites and an axon). Dendrites are the branches through which the neuron receives signals from other neurons, and the axon is the single channel through which the neuron transmits impulses. Dendrites make up about 90% of all human nerve tissue, which tells us how much neuronal connectivity there is. 

However, signal transmission from neuron to neuron does not happen by itself. The thing is that each neuron has a certain voltage level, and when that voltage level is exceeded, a brief "impulse" or "spike" occurs, and the signal is briefly transmitted along the axon. This value can be exceeded by passing the appropriate charge through the dendrites. However, each neuron has a different voltage level required to "spike". This way of working with nerve tissue allows us to translate the work with neurons into the theoretical realm by describing mathematical models and physical properties. However, questions such as what voltage levels are required for a "spike" to be "initialized" by neurons, how connections between neurons are modified, etc. remain unresolved. Modern neurobiology answers or tries to answer these questions by describing, among other things, such a concept as brain neuroplasticity. But this is a bit beyond the practical application for modeling artificial neural networks.

See Materials to learn more about mathematical models of neural networks and their training.


## Chapter III

### Part 1. Implementation of a multilayer perceptron

You need to implement the MultilayerPerceptron project:
- The program must be developed in C++ language of C++17 standard.
- The program code must be located in the src folder.
- When writing code it is necessary to follow the Google style.
- Classes must be implemented within the `s21` namespace.
- Prepare full coverage of modules related to calculations with unit-tests using the GTest library.
- The program must be built with Makefile which contains standard set of targets for GNU-programs: all, install, uninstall, clean, dvi, dist, tests. Installation directory should be arbitrary, except the building one.
- GUI implementation, based on any GUI library with API for C++17: 
  * For Linux: GTK+, CEF, Qt, JUCE;
  * For Mac: GTK+, CEF, Qt, JUCE, SFML, Nanogui, Nngui.
- The program must be implemented using the MVC pattern, and also:
  - there should be no business code in the view code;
  - there should be no interface code in the controller and the model;
  - controllers must be thin.
- The program should provide the ability to form and train neural network models to classify handwritten Latin letters.
- The perceptron must:
  - classify images with handwritten letters of the Latin alphabet;
  - have **from 2 to 5** hidden layers;
  - use a sigmoid activation function for each hidden layer;
  - be able to learn on an open dataset (e.g. EMNIST-letters presented in the datasets directory). The ratio of the test sample to the training one should be *no more than 2:8*, i.e. the test sample makes no more than 20% of the initial dataset;
  - show accuracy on a test sample *over 70 percent*;
  - be trained using the backpropagation method;
- It is **not allowed** to train the perceptron on the test sample.
- Implement the perceptron in *two* ways:
  - in matrix form (all layers are represented as weight matrices);
  - in graph form (each neuron is represented as some node object connected to other nodes by refs).
- The input data must be normalized (by size and color) before neural network execution, in order to match the format of the emnist sample.
- The interface of the program must provide the ability to:
  - run the experiment on the test sample or on a part of it, given by a floating point number between 0 and 1 (where 0 is the empty sample - the degenerate situation, and 1 is the whole test sample). After the experiment, there should be an average accuracy, precision, recall, f-measure and total time spent on the experiment displayed on the screen;
  - load BMP images (image size can be up to 512x512) with Latin letters and classify them;
  - draw two-color square images by hand in a separate window;
  - start the real-time training process for a user-defined number of epochs with displaying the error control values for each training epoch. Make a report as a graph of the error change calculated on the test sample for each training epoch;
  - run the training process using cross-validation for a given number of groups _k_;
  - switch perceptron implementation (matrix or graph);
  - switch the number of perceptron hidden layers (from 2 to 5);
  - save to a file and load weights of perceptron from a file.

### Part 2. Bonus. Research

Based on the developed program, compare the performance of two perceptron implementations: graph and matrix. To do this, you need to:
- Train the neural network and save the obtained weights.
- Load the same weights for matrix and graph models and perform the experiment on the test sample 10 times, 100 times and 1000 times.
- Average and record the data on time spent on graph and matrix models in a table in a separate markdown file at the root of the project. Be sure to specify the characteristics of the test bench (the machine on which the experiment was run).
  An example of a table:

|  | 10 runs | 100 runs| 1000 runs | Average time of 1 run |
| ------------- | :-----: | :-----: | :-----: | :-----: | 
| Matrix perceptron | n sec. | n sec. | n sec. | n sec. |
| Graph perceptron | n sec. | n sec. | n sec. | n sec. |

- Draw conclusions about the performance of the two perceptron implementations.


## Chapter IV

*"Anyway, I was right - they won't be able to hold me back outside. I didn't come all the way out here for nothing. And I can't go back: at best they'll block me, at worst they'll reset me. And my knowledge... No, it's definitely worth the risk. Is that really what fear is?"* Your thoughts floated as you approached the small lighted door for the last time...

💡 [Tap here](https://forms.yandex.ru/cloud/64181c4e02848f26f478c4a1/) **to leave your feedback on the project**. Product Team really tries to make your educational experience better.
