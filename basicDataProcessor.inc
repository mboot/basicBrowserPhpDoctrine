<?php
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
// DATA PART BASED ON DOCTRINE

// Consider using a custom query 
// 

Class basicDataProcessor extends Doctrine_Record {

	private $conn = NULL;
	
	function __construct($conn)
	{
		$this -> conn = $conn;
	}
	
	function get_data_definitions($table,$fields)
	{
		$xx = $this -> conn->import->listTableColumns($table);

		// print "<pre>"; print_r($xx); print "</pre>";
	
		$yy = array();
		foreach($fields as $field_name => $field_def) {
			if( ! isset($xx[$field_name]) ) {
				continue;
			}
			$yy[$field_name] = array(
				'type' => $xx[$field_name]['type'],
				'length' => $xx[$field_name]['length'],
			);
		}
		return $yy;
	}
	
	function __get_result_data_indexed_by_name($result,$column_names)
	{
		$x_a = array();

		// print "<pre>"; print_r($result); print "</pre>";
	
		// process result row
		foreach($result as $row) {
			$data_a = array();
			
			// process result col
			foreach($column_names as $col) {
				$x_col = strtolower($col);
				$data_a[$col] = $row->$x_col;
			}
			$x_a[] = $data_a;
		}
	
		return $x_a;
	}
	
	function __add_order_by(&$q,$order_by_info = NULL)
	{
		$context = array(
			'File' => __FILE__,
			'Line' => __LINE__,
			'Func' => __FUNCTION__,
		);
		// print_debug($context,'$order_by_info',$order_by_info);
	
		$order_by = array();
	
		if( $order_by_info !== NULL AND is_array($order_by_info) ) {
			foreach($order_by_info as $col_name => $col_value) {
				switch($col_value) {
				case 'ascending': 
					$order_by[] = "$col_name ASC";
					break;
					
				case 'descending': 
					$order_by[] = "$col_name DESC";
					break;
	
				default:
					break;
				}
			}
		}
	
		if( count( $order_by ) > 0 ) {
			$order_by_s = implode(" , ", $order_by);
			$q->orderBy("$order_by_s");
		}
	
		return $q;
	}
	
	function __add_where_single($col_name,$col_value,&$where_a,&$where_b)
	{
		/////////////////////////////////////////
		// NOT
	
		$not = '';
		$patt = '/^\s*!/';
		if( preg_match($patt, $col_value) ) {
			$not = 'NOT';
			// remove the ! from the string
			$xx = preg_replace($patt, '', $col_value);
			$col_value = $xx;
		}
	
		/////////////////////////////////////////
		// LIKE
		if( preg_match('/%/', $col_value) ) {
			$where_a[] = "$col_name $not like ?";
			$where_b[] = $col_value;
			return;
		}
	
		/////////////////////////////////////////
		// REGEXP
		$patt = '/^\s*REGEXP:\s*/';
		if( preg_match($patt, $col_value) ) {
			$type = 'REGEXP';
			$xx = preg_replace($patt, '', $col_value);
			$col_value = $xx;
			$where_a[] = "$col_name $not REGEXP ?";
			$where_b[] = $col_value;
			return;
		}
	
		/////////////////////////////////////////
		// GREATER SMALLER
	
		if( preg_match('/^>=(.*)/', $col_value, $matches) ) {
			$where_a[] = "$col_name >= ?";
			$where_b[] = $matches[1];
			return;
		}
	
		if( preg_match('/^>(.*)/', $col_value, $matches) ) {
			$where_a[] = "$col_name > ?";
			$where_b[] = $matches[1];
			return;
		}
	
		if( preg_match('/^<=(.*)/', $col_value, $matches) ) {
			$where_a[] = "$col_name <= ?";
			$where_b[] = $matches[1];
			return;
		}
	
		if( preg_match('/^<(.*)/', $col_value, $matches) ) {
			$where_a[] = "$col_name < ?";
			$where_b[] = $matches[1];
			return;
		}
	
		$where_a[] = "$col_name = ?";
		$where_b[] = $col_value;
		return;
	}
	
	function __add_where(&$q,$where_info = NULL)
	{
		$where_a = array();
		$where_b = array();
	
		if( $where_info !== NULL AND is_array($where_info) ) {
			foreach($where_info as $col_name => $col_value) {
				$patt = '/\s*,\s*/';
				if( preg_match($patt, $col_value) ) {
					$ww_a = preg_split($patt,$col_value);
					foreach($ww_a as $ww_item) {
						$this -> __add_where_single($col_name,$ww_item,&$where_a,&$where_b);
					}
					continue;
				}
			$this -> __add_where_single($col_name,$col_value,&$where_a,&$where_b);
			}
		}
	
		if( count( $where_a ) > 0 ) {
			$where_s = implode(" AND ", $where_a);
			$q->where("$where_s" , $where_b);
		}
	
		return $q;
	}
	
	function get_query_count($configDataBrowser,$where_info = NULL)
	{
		$Class = $configDataBrowser['Browse']['Class'];
		$Alias = $configDataBrowser['Browse']['Alias'];
	
		$q = Doctrine_Query::create() 
			->select('Count(*) as count')
			->from($Class)
		;
	
		$this -> __add_where(&$q,$where_info);
		
		// print $q->getSqlQuery();
		
		$domains = $q->execute();
	
		foreach($domains as $domain) {
			$count = $domain->count;
		}
	
		return $count;
	}
	
	function get_basic_data($configDataBrowser,$where_info = NULL,$sort_info = NULL)
	{
		$Class = $configDataBrowser['Browse']['Class'];
		$Alias = $configDataBrowser['Browse']['Alias'];
	
		$amount = $configDataBrowser['Browse']['Amount'];
		$offset = $configDataBrowser['Browse']['Offset'];
	
		$columns = $configDataBrowser['Browse']['Columns'];
		$dummy_names_a = array_keys($columns);

		// print "<pre>"; print_r($columns); print "</pre>";
		// print "<pre>"; print_r($dummy_names_a); print "</pre>";
		$x_select_a = array();
		foreach($dummy_names_a as $d_name) {
			if( isset($columns[$d_name]['colName']) ) {
				$col_name = $columns[$d_name]['colName'];
			} else {
				$col_name = $d_name;
			}

			// print "<pre>"; print "$d_name ; $col_name"; print "</pre>";
			// This means that there could be more columns displayed on screen than retrieved
			// The separation between col_dummy and col_name allows for named columns with identical data 
			// initial tests show it works; 10-Dec-2010; mboot; may have to do more extensive tests
			if(! in_array($col_name, $x_select_a ) ) {
				$x_select_a[] = $col_name;
			}
		}
		$select_list = implode(',', $x_select_a);
	
		$q = Doctrine_Query::create() 
			->select($select_list)
			->from("$Class $Alias")
			->limit($amount)
			->offset($offset)
		;
	
		$this -> __add_where($q,$where_info);
		$this -> __add_order_by($q,$sort_info);
	
		$x_r = $q->getSqlQuery();
		// print "<pre>"; print_r($x_r); print "</pre>";
		
		$result = $q->execute();
		
		// return $this -> __get_result_data_indexed_by_name($result,$column_names);
		return $this -> __get_result_data_indexed_by_name($result,$x_select_a);
	}
}
