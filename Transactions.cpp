#include "Transactions.hpp"


void Transactions::readInput(){
	/*
        -->this function read the given input file and store all the transactions details in d.s. 'transactions'
         
	*/
	int Tid, DataItem=0;
    char Opr;
    while(cin>>Tid){
        cin>>Opr;
        if(Opr!='C')
            cin>>DataItem;
        else{
            commit[Tid]=transaction.size();
        }
        // Not covering if input format is wrong.
	//what is DataItem for operation commit;is it required?    
        transaction.push_back({Tid,{Opr,DataItem}});
    }
}


void Transactions::PossibleChains(){
	/*
	    --> this method find all the possible chains and store it in d.s. 'chain'
	    --> each chain represents collection of read operations on a dataitem that is written by a transaction before those read operations
        
	*/
	vector<int> temp;
	pair<int,int> firstRead;
	for(int i=0; i<transaction.size(); i++){
		//finds a write operation and then search for the chain of reads on that resource untill next write it observed on that resource
		if(transaction[i].second.first!='W')
		    continue;
		//found a write
		int DataItem=transaction[i].second.second;
		
		for(int j=i+1; j<transaction.size(); j++){
		    //finds the chain of reads corresponding to curr resource from above parent loop resource
		    if(transaction[j].second.second!=DataItem)
			continue;
		    //found same resource
		    if(transaction[j].second.first=='R'){
			    +
			if(temp.size()==0)
				firstRead={transaction[j].first,j};
			temp.push_back(transaction[j].first);
		    }
		    //a new write on curr resource observed so break;
		    if(transaction[j].second.first=='W')
			break;
		}
		if(temp.size()){
		    chain.push_back({transaction[i].first,temp});//(transaction id of writing transaction, transaction id of reading transaction after that write but before next write on the same resource)
		    firstReadArray.push_back({transaction[i].first,{firstRead}});
		}
		temp.clear();
    	}
}


void Transactions::isRecoverable(){
	/*
         --> this method checks whether the given schedule is recoverable or not
	*/
	for(int i=0; i<chain.size(); i++){
		int x = commit[chain[i].first];//commit time of current write 
		for(int j=0; j<chain[i].second.size(); j++){
		    //traversing chain to find reads commited before current write	
		    if(x>=commit[chain[i].second[j]]){
			//found a transaction reading the content(present in the chain)commmit before the commit of transaction writting the resource
			cout<<"No"<<endl;
			cout<<"Transaction "<<chain[i].second[j]<<" is reading DataItem written by Transaction "<<chain[i].first<<endl;
			cout<<"And Transaction "<<chain[i].first<<" commits after Transaction "<<chain[i].second[j]<<endl;
			return;
		    }
		}
	}
	cout<<"Yes"<<endl;
}


void Transactions::isCascadeless(){
	/*
         --> this method checks whether the given schedule is cascadeless or not
	*/
	for(int i=0; i<firstReadArray.size(); i++){
        int x = commit[firstReadArray[i].first];
        if(x>=firstReadArray[i].second.second){
        	cout<<"No"<<endl;
        	cout<<"Transaction "<<firstReadArray[i].second.first<<" is reading DataItem written by Transaction "<<firstReadArray[i].first<<endl;
        	cout<<"And Transaction "<<firstReadArray[i].first<<" commits after reading by Transaction "<<firstReadArray[i].second.first<<endl;
        	return;
        }
    }
    cout<<"Yes"<<endl;
}

