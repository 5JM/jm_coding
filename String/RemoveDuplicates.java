public class RemoveDuplicates {
    // Remove Duplicates from Sorted Array
    public static void main(String[] args){
        int[] arr = {1,1,2,3};
        for(int numbers : arr) System.out.print(numbers);
        System.out.println();
        System.out.println(arr.length);
        System.out.println( removeDuplicates(arr));
        return;
    }
    //i j       i   j       i j       i   j       i j
    //1 1 2 3 - 1 1 2 3 - 1 2 2 3 - 1 2 2 3 - 1 2 3 3

    //i j         ij        i j       i   j       i j
    //1 2 2 3 - 1 2 2 3 - 1 2 2 3 - 1 2 2 3 - 1 2 3 3
    public static int removeDuplicates(int [] nums){
        if(nums.length < 2)
            return nums.length;
        int i =0;
        //two pointer
        //i increases only when i, j elements are equal.
        for(int j=1;j<nums.length;j++){
            if(nums[i]!=nums[j]){
                i++;
                //Stores the next element in place of the duplicate element.
                nums[i] = nums[j];
            }
        }
        return i+1;
    }
}
