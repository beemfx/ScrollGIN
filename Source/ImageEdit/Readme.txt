ImageEdit, ImageLib Copyright (c) 2003, Blaine Myers

About:

Despite it's name ImageEdit doesn't actually edit images.  Rather it edits image databases (.ida), and has the ability to build (as well as unbuild or "import") image libraries (.ilb).

Usage:
Simply double click the application to start it up.  You can begin editing right now, except that there is no image in the database, so it is a good idea to select Edit|Bitmaps... and add a bitmap to the current archive.

Once a bitmap is in the archive you can change the current entry by entering the new values in the available boxes, then press apply.  To change to another entry use the scrollbar.  To add another entry, use the Add Entry button.  When you add a new entry it will start with bitmap 1, 1 frames, and all values set to zero.

From the Edit|Bitmaps... dialog you can add up to 10 bitmaps (.bmp, .dib).  Note which number is in front of which bitmap name.

The Edit|Autogenerate will allow you to automatically generate a series of images.  This is useful if you have a bitmap full of tiles all equally spaced.  This way you do not have to type in each tile separately.

Boxes:

Bitmap #:
This specifies the bitmap number the selected image will be part of.  You can find out which bitmap is assigned to which number by using the Edit|Bitmaps... dialog.

Frames:
Indicates how many frames are in the current sprite.  Not all applications will use this value.  If your not sure what a sprite is you probably aren't using this app anyway.  Note: an image that is part of a sprite and not the first image should have a frames value of zero.

Source X: 
This is the x location on the source bitmap that the image begins.

Source Y:
This is the y location on the source bitmap that the image begins. (Y starts from the top going down.)

Source Width:
This is the width of the image on the source bitmap.

Source Height:
This is the height of the image on the source bitmap.

Width:
This is the width the image will be drawn, the image will be stretched or compressed if not the same as source width.

Height:
This is the hieght the image will be draw, the image will be stretched or compressed if not the same as source height.

Image Label:
This is the name for the image.  Certain applications can use this value to get a pointer to the image.  Not all applications use this value.


Additional Notes:
This application isn't complete.  But it is complete to the point that I probably won't work on it again.  Currently there is no way to delete an entry and there is no way to change or remove a bitmap.  If you change a value then switch to another image without pressing apply the new value will not be saved, and you will not recieve a warning.

For best results put any bitmaps being used, and always save databases in the same directory as ImageEdit.

ImageEdit requires ImageLib.dll in order to function (and it should be included with the package).