# Color Stuff

Testing a way to reduce a PNG to a small set of colors based on what colors make sense to keep, and decompose it into a set of layers for each color, with the goal of turning each layer into a simple shape to convert into a lightweight SVG file.

### Current status
---

Reduces a picture's color palette to 16 colors using KPP and KMeans based on the RGB colors used in the picture and their frequency, then split into layers each containing one color only. Picture with the reduced palette and the palette in the pain.net format are saved in the Output folder, layers are saved in the folder Output/Layers. If the folders don't exist they are created.  

### To do list
---

Implement a better algorithm for reducing the palette([1](#ref1), [2](#ref2))  
Fill a "depth buffer" with each layer with top layer being the least represented one  
Link "islands" from lower layers between each other using depth buffer, to eliminate isolated dots and reduce number of edges
Make command arguments to make it usable outside of changing the main code  

### Resources
---

`zarro.png` is a small portion of the artwork for Lappland the Decadenza, from Arknights, sourced from the [wiki.gg page](https://arknights.wiki.gg/wiki/Lappland_the_Decadenza). `png.txt` is this png in readable hexadecimal values, for debug purposes.  

This project uses [stb_image and stb_image_write](https://github.com/nothings/stb) for image loading and writing. Homemade alternative for image writing in the PAM and BMP formats (No alpha support).  

<a id="ref1">[1 - PKH, Improving Color Quantization Heuristics](https://blog.pkh.me/p/39-improving-color-quantization-heuristics.html) </a>  
<a id="ref2">[2 - StackExchange, contrast calculations](https://gamedev.stackexchange.com/questions/38536/given-a-rgb-color-x-how-to-find-the-most-contrasting-color-y)</a>
