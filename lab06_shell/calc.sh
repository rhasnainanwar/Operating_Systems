while : ; do
	echo "Enter first number:"
	read num1
	echo "Enter second number:"
	read num2
	
	echo "Which operation do you want to perform?"
	read op
	
	if [[ $op == "+" ]]; then
		echo "Ans: $(($num1 + $num2))"
	elif [[ $op == "-" ]]
	then
		echo "Ans: $(($num1 - $num2))"
	elif [[ $op == "*" ]]
	then
		echo "Ans: $(($num1 * $num2))"
	elif [[ $op == "/" ]]
	then
		echo "Ans: $(($num1 / $num2))"
	fi
	
	echo ""
	echo "Again? [y/n]"
	read choice
	
	if [[ $choice == "y" ]]; then
		continue
	else
		break
	fi
done
