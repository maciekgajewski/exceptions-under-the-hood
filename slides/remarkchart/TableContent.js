/**
 Create TableContent by reading date from given table.
 @class
 @classdesc Contains table content and color configuration.
 @param {object} - The table to read data from.
 @param {string} - The chart type.
 @param {object} - The known colors.
 @since v0.1.0
 */
function TableContent(table, chartType, knownColors) {
	/**
	 Loads content from given table.
	 @param {object} - The chart table.
	 @returns {object} - The the table content.
	 */
	var readContentFromTable = function(table) {
		var lines = [];
		table.find("tr").each(function(index, tr) {
			var line = $('th, td', tr).map(function(index, td) {
				return $(td).text();
			});
			lines[index] = line;
		});
		return lines;
	};

	/**
	 Loads explicit color configuration from given table.
	 @param {object} - The chart table.
	 @param {string} - The chart type.
	 @param {object} - The known colors.
	 @returns {Array<ChartColor>} The explicit colors (can contain empty values).
	 */
	var readColorsFromTable = function(table, chartType, knownColors) {
		var elementClasses = [];

		if (['chart-doughnut', 'chart-pie', 'chart-polararea'].indexOf(chartType) != -1) {
			table.find("th span").each(function(index, span) {
				elementClasses[index] = $(span).prop('class');
			});
		} else {
			table.find('tbody tr td:first-child span').each(function(index, span) {
				elementClasses[index] = $(span).prop('class');
			});
		}

		var colors = [];
		$(elementClasses).each(function(index, classes) {
			$.each(classes.split(' '), function(spanindex, singleClass) {
				var matchingColor = knownColors[singleClass];
				if ( typeof matchingColor !== "undefined") {
					colors[index] = matchingColor;
					return false;
				}
			});
		});
		return colors;
	};

	this.content = readContentFromTable(table);
	this.colors = readColorsFromTable(table, chartType, knownColors);
}