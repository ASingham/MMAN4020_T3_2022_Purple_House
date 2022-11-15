import Alert from '@mui/material/Alert';
import { CartesianGrid, Legend, Line, LineChart, ReferenceLine, ResponsiveContainer, Tooltip, XAxis, YAxis } from 'recharts';

const Graph = ({ data, maxTemp }) => {
  const CustomLabel = ({ active, payload }) => {
    if (active && payload && payload.length) {
      return (
        <Alert severity={payload[0].value >= maxTemp ? 'error' : 'info'}>
          <strong>{`${payload[0].value} ${payload[0].unit}`}</strong>
          <div>{payload[0].payload.date}</div>
          <div>{payload[0].payload.time}</div>
        </Alert>
      );
    }
    return null;
  };

  const WarningDot = ({ value, cx, cy }) => {
    if (value >= maxTemp) return (

      <svg x={cx - 5} y={cy - 5} width={10} height={10}>
        <g transform="translate(5 5)">
          <circle r='5' fill='white'/>
          <circle r='4' fill='#f44336'/>
        </g>
      </svg>
    );
    return (
      <svg x={cx - 5} y={cy - 5} width={10} height={10}>
        <g transform="translate(5 5)">
          <circle r='5' fill='white'/>
          <circle r='4' fill='#66bb6a'/>
        </g>
      </svg>
    );
  };

  return (
    <>
      { data.length > 0 &&
        <ResponsiveContainer width='100%' height={500}>
          <LineChart data={data} margin={{ top: 10, right: 10, left: 10, bottom: 10 }}>
            <Line name='Temperature' type='linear' dataKey='temp' unit='°C' dot={<WarningDot/>} stroke='#ce93d8'/>
            <Legend verticalAlign="bottom" height={36}/>
            <CartesianGrid stroke='#ccc' strokeDasharray="5 10" vertical={false}/>
            <XAxis dataKey='time' interval='preserveStartEnd' tickCount={7}/>
            <YAxis type='number' domain={['dataMin-0.5', 'dataMax+0.5']} unit='°C'/>
            <ReferenceLine y={maxTemp} stroke='red' strokeDasharray='9 5'/>
            <Tooltip content={<CustomLabel/>}/>
          </LineChart>
        </ResponsiveContainer>
      }
    </>
  );
};

export default Graph;
