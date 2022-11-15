import moment from 'moment';

class CSVParser {
  static parseAskSensors(results) {
    let data = [];
    results.data.forEach((row, idx) => {
      const date = new Date(row[results.meta.fields[0]]);
      data = [
        {
          'datetime': date,
          'date': date.toDateString(),
          'time': moment(date).format('h:mm A'),
          'temp': +row[results.meta.fields[1]].split(',')[0],
          'id': results.data.length - idx
        },
        ...data
      ];
    });
    results.data = data;
    results.meta.name = results.meta.fields[1];
    results.meta.timezone = results.meta.fields[0].replace('Date (', '').replace(')', '');
    results.meta.fields = ['Date', 'Time', 'Temperature'];
    return results;
  }
}

export default CSVParser;
