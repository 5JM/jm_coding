public class twoSum {
    public static void main(String args[]){
        int[]test={3,3};
        int[] arr = twoSum(test,6);
        System.out.println(arr[0]+", "+arr[1]);
        return;
    }
    static int[] twoSum(int[] nums, int target){
        for(int i=0;i<nums.length-1;i++){
            for(int j=i+1;j<nums.length;j++){
                if(target-nums[i]==nums[j]) {
                    return new int[] {i,j};
                }
            }
        }
        throw new IllegalArgumentException("No sol");
    }
}
