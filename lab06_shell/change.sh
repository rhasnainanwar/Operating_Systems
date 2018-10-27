for file in $(ls *txt)
do
	echo $file
	#remove tags
	sed -i 's/<[^>]*>//g' $file
	#acronums
	sed -i 's/NUST/National University of Sciences and Technology/g' $file
	sed -i 's/SEECS/School of Electrical Engineering and Computer Science/g' $file
	sed -i 's/IT/Information Technology/g' $file
	
	#change case
	tr A-Z a-z < $file > "mod"
	mv "mod" $file
	
	#get numbers
	numbers=$(grep -Eo '[0-9]+' $file)
	
	# for each number
	for num in $numbers
	do
		# replace with words
		perl -i -p -e "s/$num/$(perl -e 'use Lingua::EN::Numbers qw(num2en);print num2en('$num')')/g;" $file
	done
done
