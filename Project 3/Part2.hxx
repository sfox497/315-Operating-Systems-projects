#include <iostream>
#include <fstream>

//create(char name[8], int32 size): create a new file with this name and with these many blocks. (We shall assume that the file size is specified at file creation time and the file does not grow or shrink from this point on)
//delete(char name[8]): delete the file with this name
//read(char name[8], int32 blockNum, char buf[1024]): read the specified block from this file into the specified buffer; blockNum can range from 0 to 7.
//write(char name[8], int32 blockNum, char buf[1024]): write the data in the buffer to the specified block in this file.
//ls(void): list the names of all files in the file system and their sizes.

class myFileSystem{

    public myFileSystem(string diskName){
      // Open the file with name diskName
      ofstream file;
      file.open(diskName);
    
      // Read the first 1KB and parse it to structs/objecs representing the super block
      // 	An easy way to work with the 1KB memory chunk is to move a pointer to a
      //	position where a struct/object begins. You can use the sizeof operator to help
      //	cleanly determine the position. Next, cast the pointer to a pointer of the
      //	struct/object type.

      // Be sure to close the file in a destructor or otherwise before
      // the process exits.
      file.close();
    }


    public int create(char name[8], int32 size){ //create a file with this name and this size

      // high level pseudo code for creating a new file

      // Step 1: Look for a free inode by searching the collection of objects
      // representing inodes within the super block object.
      // If none exist, then return an error.
      // Also make sure that no other file in use with the same name exists.

      // Step 2: Look for a number of free blocks equal to the size variable
      // passed to this method. If not enough free blocks exist, then return an error.

      // Step 3: Now we know we have an inode and free blocks necessary to
      // create the file. So mark the inode and blocks as used and update the rest of
      // the information in the inode.

      // Step 4: Write the entire super block back to disk.
      //	An easy way to do this is to seek to the beginning of the disk
      //	and write the 1KB memory chunk.
    } // End Create



    public int delete(char name[8]){
    // Delete the file with this name

    // Step 1: Look for an inode that is in use with given name
    // by searching the collection of objects
    // representing inodes within the super block object.
    // If it does not exist, then return an error.

    // Step 2: Free blocks of the file being deleted by updating
    // the object representing the free block list in the super block object.

    // Step 3: Mark inode as free.

    // Step 4: Write the entire super block back to disk.

    } // End Delete


    public int ls(void){ 
    // List names of all files on disk

    // Step 1: Print the name and size fields of all used inodes.

    } // End ls

    public int read(char name[8], int32 blockNum, char buf[1024]){

    // read this block from this file
    // Return an error if and when appropriate. For instance, make sure
    // blockNum does not exceed size-1.

    // Step 1: Locate the inode for this file as in Step 1 of delete.

    // Step 2: Seek to blockPointers[blockNum] and read the block
    // from disk to buf.

    } // End read


    public int write(char name[8], int32 blockNum, char buf[1024]){

    // write this block to this file
    // Return an error if and when appropriate.

    // Step 1: Locate the inode for this file as in Step 1 of delete.

    // Step 2: Seek to blockPointers[blockNum] and write buf to disk.
    
    } // end write

}

int main(int argc, char *argv[]){
	return 0;
}
