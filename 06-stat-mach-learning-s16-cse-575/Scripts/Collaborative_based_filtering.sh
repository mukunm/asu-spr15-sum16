echo "Starting script"
cleanUprmseFolder="hadoop fs -rmr /rmse"
cleanUpalstrainingFolder="hadoop fs -rmr /alstraining"
cleanUpuserFolder="hadoop fs -rmr /user"
getRMSE="hadoop fs -cat /rmse/*"
arrX=''
for itr in 1 5 10 50;
do
	echo "***************** Running for numFeatures = $itr *************"	
	echo "cleanup rmse folder if exists"
	eval $cleanUprmseFolder
	echo "cleanup alstraining folder if exists"
	eval $cleanUpalstrainingFolder
	echo "cleanup user folder if exists"
	eval $cleanUpuserFolder

	#train using itr number of features
	mahouttrain="mahout parallelALS --input hdfs://master:54310/split/trainingSet/ --output hdfs://master:54310/alstraining/ --numFeatures $itr --numIterations 5 --lambda 0.065"
	eval $mahouttrain
	#evaluate and calculate rmse
	mahoutevaluate="mahout evaluateFactorization --input hdfs://master:54310/split/probeSet --output hdfs://master:54310/rmse/ --userFeatures hdfs://master:54310/alstraining/U/ --itemFeatures hdfs://master:54310/alstraining/M/"
	echo "*************** Trained Model for numFeatures = $itr **************"	
	eval $mahoutevaluate
	#fetch rmse and store it in list
	arrX=$arrX$(eval $getRMSE)','
	echo "*************** Calculated RMSE and appended to a list ******************"
	echo "Present list looks like "
	echo $arrX
done
echo $arrX
