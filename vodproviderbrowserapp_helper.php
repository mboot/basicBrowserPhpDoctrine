<?php
// Example setup for the basic broswer, need a mini application later to show how it works, without CI

////////////////////////////////////////////////////////
// The Doctrine Query Wrapper
require_once dirname(__FILE__) . '/' . "basicDataProcessor.inc";

////////////////////////////////////////////////////////
// The browser Render object that will handle all interactions with the user
require_once dirname(__FILE__) . '/' . "basicBrowserRender.inc";

///////////////////////////////////////////////////
///////////////////////////////////////////////////

Class VodProviderBrowserApp extends basicBrowserRender 
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
	
			'Table' => 'vodProvider',
			'Class' => 'VodProvider',
			'Alias' => 'v',
		
			'Columns' => array(
	
				'vodProviderUniqueCode' => array(
					'showLabel' => 'Code',
					'sortable' => TRUE,
					'searchable' => TRUE,
					// The code will be the connector to the view/edit applet
					'islink' => array(
						'callback' => array(
							'function' => 'mk_link_vodProviderCode',
							'params' => array(
								'vodProviderUniqueCode',
							),
						),
					),
				),
				'vodprovidername' => array(
					'showLabel' => 'Name',
					'sortable' => TRUE,
					'searchable' => TRUE,
				),
				'vodProviderDescription' => array(
					'showLabel' => 'Description',
					'sortable' => FALSE,
					'searchable' => FALSE,
				),
				'vodProviderAttributes' => array(
					'showLabel' => 'Attributes',
					'sortable' => FALSE,
					'searchable' => FALSE,
				),
			),
		),
	);

	public function __construct($conn)
	{
		parent::__construct($conn);
	}

	public function mk_link_vodProviderCode($params) 
	{
		// This is a call back function
		// params is array( name => data )
	
		if( $params['vodProviderUniqueCode'] ) {
			$x = $params['vodProviderUniqueCode'];
			$href = sprintf("/index.php/vproviders/edit/%s", $x );

			$data = <<<EOS
			<a href="$href" class="link_data" > $x </a>
EOS;
			return $data;
		}
	
		return 'ERROR';
	}
}

function vodProviderBrowserApp() 
{
	// make function so CodeIgniter can Call me as a helper
	// Replace the actual connect data before placing on github
	Doctrine_Manager::getInstance()->bindComponent('TableName', 'ConnectionName');
	$conn = Doctrine_Manager::connection();
	//////////////////////////
	// Setup the Browser Applet
	$VodProviderBrowserApp = New VodProviderBrowserApp($conn);

	// Interact with the user
	$html = $VodProviderBrowserApp -> basicBrowser();

	return $html;
}

