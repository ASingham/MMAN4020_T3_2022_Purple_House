import moment from 'moment';

class CSVParser {
  static parseAskSensors(name, results) {
    let data = [];
    results.data.forEach((row, idx) => {
      const date = new Date(row[results.meta.fields[0]]);
      let temp = '';
      if (String(row[results.meta.fields[1]]).indexOf(',') > -1) {
        temp = +row[results.meta.fields[1]].split(',')[0];
      } else {
        temp = +row[results.meta.fields[1]];
      }
      
      data = [
        {
          'datetime': date,
          'date': date.toDateString(),
          'time': moment(date).format('h:mm:ss A'),
          'temp': temp,
          'id': results.data.length - idx
        },
        ...data
      ];
    });
    results.data = data;
    results.meta.name = name.replace('.csv', '');
    results.meta.timezone = results.meta.fields[0].replace('Date (', '').replace(')', '');
    results.meta.fields = ['Date', 'Time', 'Temperature'];
    return results;
  }
}

export default CSVParser;
