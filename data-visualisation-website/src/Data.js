import moment from 'moment';

const average = (data) => {
  return +(data.reduce((total, d) => {
    return total + d.temp;
  }, 0) / data.length).toFixed(2);
};

const dataFromSameDay = (data, day) => {
  return data.filter(d => moment(d.datetime).isSame(day, 'day'));
};

const dataFromSameMonth = (data, month) => {
  return data.filter(d => moment(d.datetime).isSame(month, 'month'));
};

class Data {
  constructor(results) {
    this.empty = results?.data.length ? false : true;
    this.name = this.empty ? undefined : results.meta.name;
    this.fields = this.empty ? undefined : results.meta.fields;
    this.data = this.empty ? undefined : results.data;
    this.statsToday = this.empty ? undefined : this.calcStatsToday();
    this.dataLast24Hours = this.empty ? undefined : this.last24Hours();
    this.dataWeek = this.empty ? undefined : this.calcDataWeek();
    this.dataYear = this.empty ? undefined : this.calcDataYear();
  }

  calcStatsToday() {
    const data = this.data.filter(d => moment(d.datetime).isSame(moment(), 'day'));
    if (!data.length) return [];
    const min = data.reduce((prev, curr) => {
      return prev.temp < curr.temp ? prev : curr;
    }, { temp: Number.MAX_SAFE_INTEGER });
    min.temp = +(min.temp).toFixed(2);
    
    const max = data.reduce((prev, curr) => {
      return prev.temp > curr.temp ? prev : curr;
    }, { temp: Number.MIN_SAFE_INTEGER });
    max.temp = +(max.temp).toFixed(2);

    const avg = +(data.reduce((total, d) => {
      return total + d.temp;
    }, 0) / data.length).toFixed(2);

    return { data, min, max, avg };
  }
  
  calcDataWeek() {
    if (this.data.empty) return [];
    const weekStart = moment().startOf('week');
    const avgs = [];
    for (let i = 0; i < 7; i++) {
      const avgTemp = average(dataFromSameDay(this.data, weekStart));
      if (!isNaN(avgTemp)) {
        avgs.push({ temp: avgTemp, time: weekStart.format('ddd') });
      }
      weekStart.add(1, 'days');
    }
    return avgs;
  }

  calcDataYear() {
    if (this.data.empty) return [];
    const yearStart = moment().startOf('year');
    const avgs = [];
    for (let i = 0; i < 12; i++) {
      const avgTemp = average(dataFromSameMonth(this.data, yearStart));
      if (!isNaN(avgTemp)) {
        avgs.push({ temp: avgTemp, time: yearStart.format('MMM') });
      }
      yearStart.add(1, 'months');
    }
    return avgs;
  }

  last24Hours() {
    const data = this.data.filter(d => moment(d.datetime).isSameOrAfter(moment().subtract(24, 'hours')));
    if (!data.length) return [];
    return data;
  }

  getDataFromDate(date) {
    const data = this.data.filter(d => moment(d.datetime).isSame(moment(date, 'YYYY-MM-DD'), 'day'));
    if (!data.length) return [];
    return data;
  }
}

export default Data;
