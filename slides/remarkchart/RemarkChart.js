/**
 Initialises chart generation on given slideshow.
 @class
 @classdesc Provides chart generation.
 @param {object} - the remark.js slideshow
 @param [options] {object} - options
 @since v0.1.0
 @example
 ```js
 var remarkchart = new RemarkChart(slideshow, options);
 ```
 */
function RemarkChart(slideshow, options) {
	/**
	 The chart colors
	 @default {'butter', 'orange', 'chocolate' ,'chameleon', 'skyblue', 'plum', 'scarletred', 'aluminium'}
	 @type {Object.<string, ChartColor>}
	 @since v0.1.0
	 */
	var colors = {
		'butter' : new ChartColor('#edd400', '#fce94f'),
		'orange' : new ChartColor('#f57900', '#fcaf3e'),
		'chocolate' : new ChartColor('#c17d11', '#e9b96e'),
		'chameleon' : new ChartColor('#73d216', '#8ae234'),
		'skyblue' : new ChartColor('#3465a4', '#729fcf'),
		'plum' : new ChartColor('#75507b', '#ad7fa8'),
		'scarletred' : new ChartColor('#cc0000', '#ef2929'),
		'aluminium' : new ChartColor('#555753', '#888a85')
	};

	/**
	 The implicit colors.
	 @default
	 @type {Array<ChartColor>}
	 @since v0.1.0
	 */
	var defcolors = [colors.butter, colors.orange, colors.chocolate, colors.chameleon, colors.skyblue, colors.plum, colors.scarletred, colors.aluminium];

	/**
	 Determines if current browser is a mobile one.
	 @since v0.1.0
	 */
	var isMobile = {
		Android : function() {
			return navigator.userAgent.match(/Android/i);
		},
		BlackBerry : function() {
			return navigator.userAgent.match(/BlackBerry/i);
		},
		iOS : function() {
			return navigator.userAgent.match(/iPhone|iPad|iPod/i);
		},
		Opera : function() {
			return navigator.userAgent.match(/Opera Mini/i);
		},
		Windows : function() {
			return navigator.userAgent.match(/IEMobile/i);
		},
		/**
		 Determines if current browser is a mobile one.
		 @returns {boolean} true if it is a mobile browser, false if not.
		 @since v0.1.0
		 */
		any : function() {
			return (isMobile.Android() || isMobile.BlackBerry() || isMobile.iOS() || isMobile.Opera() || isMobile.Windows());
		}
	};

	/**
	 Converts tables to charts on visible slides (slide and presenter preview).
	 @since v0.1.0
	 */
	var initVisibleSlide = function() {
		var visibleSlides = '.remark-visible, .remark-presenter-mode .remark-preview-area';
		var anyChart = '.chart-doughnut, .chart-pie, .chart-bar, .chart-polararea, .chart-line, .chart-radar';
		$(visibleSlides).find(anyChart).each(function(index, tableDiv) {
			var jTableDiv = $(tableDiv);

			if (!jTableDiv.hasClass('chart-initialized')) {
				var chartType = getChartType(tableDiv);
				createChart(tableDiv, chartType);
				jTableDiv.toggleClass("chart-initialized", true);
			}
		});

		$('.remark-presenter-mode .remark-preview-area').find(anyChart).each(function(index, tableDiv) {
			// Seems the preview area is a copy of the slide dom tree which might have the chart canvas
			// and 'chart-initialized' class, but never the javascript to draw the chart.
			// Thus the chart always has to be recreated here.
			var chartType = getChartType(tableDiv);
			createChart(tableDiv, chartType);
		});
	};

	/**
	 Creates a chart of given chart type for the table contained in given div.
	 @param {object} - The `<div>`containing the table.
	 @param {object} - The type of the chart.
	 */
	function createChart(tableDiv, chartType) {
		var table = $(tableDiv).find('table');

		// Use the existing canvas on preview area if it exists.
		var canvas = $(tableDiv).find('canvas:first');
		if (canvas.length === 0) {
			canvas = $('<canvas>');
			canvas.appendTo(tableDiv);
		}

		var ctx = canvas.get(0).getContext("2d");

		var chart;
		var data;
		switch(chartType) {
		case 'chart-doughnut':
			data = createChartDataFromTable(table, chartType, colors);
			chart = new Chart(ctx).Doughnut(data);
			break;
		case 'chart-pie':
			data = createChartDataFromTable(table, chartType, colors);
			chart = new Chart(ctx).Pie(data);
			break;
		case 'chart-bar':
			data = createChartDataFromTable2D(table, chartType, colors);
			chart = new Chart(ctx).Bar(data);
			break;
		case 'chart-line':
			data = createChartDataFromTable2D(table, chartType, colors);
			chart = new Chart(ctx).Line(data);
			break;
		case 'chart-radar':
			data = createChartDataFromTable2D(table, chartType, colors);
			chart = new Chart(ctx).Radar(data);
			break;
		case 'chart-polararea':
			data = createChartDataFromTable(table, chartType, colors);
			chart = new Chart(ctx).PolarArea(data);
			break;
		}

		table.hide();
		$(tableDiv).data('chart', chart);
	}

	function createChartDataFromTable(table, chartType, colors) {
		var tableContent = new TableContent(table, chartType, colors);

		var customColors = defcolors.slice();
		for (var x = 0; x < tableContent.colors.length; x++) {
			if ( typeof tableContent.colors[x] !== "undefined") {
				customColors[x] = tableContent.colors[x];
			}
		}

		var data = [];
		for (var i = 0; i < tableContent.content[0].length; i++) {
			var chartelem = {
				value : parseFloat(tableContent.content[1][i]),
				label : tableContent.content[0][i]
			};
			$.extend(chartelem, customColors[i]);
			data[i] = chartelem;
		}
		return data;
	}

	function createChartDataFromTable2D(table, chartType, colors) {
		var tableContent = new TableContent(table, chartType, colors);

		var customColors = defcolors.slice();
		for (var colorIndex = 0; colorIndex < tableContent.colors.length; colorIndex++) {
			if ( typeof tableContent.colors[colorIndex] !== "undefined") {
				customColors[colorIndex] = tableContent.colors[colorIndex];
			}
		}

		var data = {
			labels : tableContent.content[0].slice(1),
			datasets : []
		};
		for (var contentIndex = 1; contentIndex < tableContent.content.length; contentIndex++) {
			var chartelem = {
				label : tableContent.content[contentIndex][0],
				data : []
			};
			$.extend(chartelem, customColors[contentIndex - 1]);
			for (var labelIndex = 0; labelIndex < data.labels.length; labelIndex++) {
				chartelem.data[labelIndex] = parseFloat(tableContent.content[contentIndex][labelIndex + 1]);
			}
			data.datasets[contentIndex - 1] = chartelem;
		}
		return data;
	}

	/**
	 Configures global chart defaults.
	 @since v0.1.0
	 */
	function setGlobalConfig() {
		var globalConfig = {
			showTooltips : !isMobile.any(),
			animation : false,
			responsive : true
		};
		$.extend(Chart.defaults.global, globalConfig);
	}

	/**
	 Returns the chart type of given element (determined by elements style class).
	 @param {object} - The `<div>` tagged with a chart class.
	 @returns {string} the first matching chart type if exists.
	 @since v0.1.0
	 */
	function getChartType(element) {
		var chartTypes = ['chart-doughnut', 'chart-pie', 'chart-bar', 'chart-polararea', 'chart-line', 'chart-radar'];
		var classList = $(element).attr('class').split(/\s+/);

		for (var i = 0; i < classList.length; i++) {
			if (chartTypes.indexOf(classList[i]) != -1) {
				return classList[i];
			}
		}
	}

	setGlobalConfig();
	slideshow.on('afterShowSlide', function(slide) {
		initVisibleSlide();
	});
	initVisibleSlide();
}