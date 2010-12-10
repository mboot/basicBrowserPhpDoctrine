<?php
////////////////////////////////////////////////////////
// The Doctrine Query Wrapper
require_once dirname(__FILE__) . '/' . "basicDataProcessor.inc";

////////////////////////////////////////////////////////
// The browser Render object that will handle all interactions with the user
require_once dirname(__FILE__) . '/' . "basicBrowserRender.inc";

///////////////////////////////////////////////////
///////////////////////////////////////////////////

Class VodAssetBrowserApp extends basicBrowserRender 
{
	protected $basicBrowserConfig = array(
		'Defaults' => array(
			'Browse' => array(
				'Amount' => 25,
			),
		),
		'Browse' => array(
			'Title' => '',
	
			'Amount' => 25,
			'Offset' => 0,
			'Total' => 0,
	
			'Table' => 'vodAsset',
			'Class' => 'VodAsset',
			'Alias' => 'va',
		
			'Columns' => array(
	
				'vodAssetId1' => array(
					'showLabel' => 'Edit',
					'sortable' => FALSE,
					'searchable' => FALSE,
					'colName' => 'vodAssetId',
					// The code will be the connector to the view/edit applet
					'islink' => array(
						'callback' => array(
							'function' => 'mk_link_vodAssetId',
							'params' => array(
								'vodAssetId',
							),
						),
					),
				),
				'vodAssetId2' => array(
					'showLabel' => 'Delete',
					'sortable' => FALSE,
					'searchable' => FALSE,
					'colName' => 'vodAssetId',
					// The code will be the connector to the view/edit applet
					'islink' => array(
						'callback' => array(
							'function' => 'mk_link_vodAssetId',
							'params' => array(
								'vodAssetId',
							),
						),
					),
				),
				'vodAssetMediaId' => array(
					'showLabel' => 'MediaId',
					'sortable' => TRUE,
					'searchable' => TRUE,
				),
				'vodAssetTitle' => array(
					'showLabel' => 'Title',
					'sortable' => TRUE,
					'searchable' => TRUE,
					'maxLen' => 40,
				),
				'vodAssetStatus' => array(
					'showLabel' => 'Status',
					'sortable' => TRUE,
					'searchable' => TRUE,
				),
				'vodAssetIngestSource' => array(
					'showLabel' => 'Source',
					'sortable' => TRUE,
					'searchable' => TRUE,
				),
				'vodAssetHasPromo' => array(
					'showLabel' => 'Promo',
					'sortable' => TRUE,
					'searchable' => TRUE,
				),
			),
		),
	);

	public function __construct($conn)
	{
		parent::__construct($conn);
	}

	public function mk_link_vodAssetId($params) 
	{
		// This is a call back function
		// params is array( name => data )
	
		if( $params['vodAssetId'] ) {
			$x = $params['vodAssetId'];
			$href = sprintf("/index.php/vasset/edit/%s", $x );

			$data = <<<EOS
			<a href="$href" class="link_data" > $x </a>
EOS;
			return $data;
		}
	
		return 'ERROR';
	}
}

function vodAssetBrowserApp() 
{
	// Fucntion to interface with CodeIgniter
	// replaced the actual connection data
	Doctrine_Manager::getInstance()->bindComponent('TableName', 'connection_name');
	$conn = Doctrine_Manager::connection();
	//////////////////////////
	// Setup the Browser Applet
	$VodAssetBrowserApp = New VodAssetBrowserApp($conn);

	// Interact with the user
	$html = $VodAssetBrowserApp -> basicBrowser();

	return $html;
}

