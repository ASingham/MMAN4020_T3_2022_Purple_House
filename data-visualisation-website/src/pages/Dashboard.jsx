import Box from '@mui/material/Box';
import Chip from '@mui/material/Chip';
import Divider from '@mui/material/Divider';
import { styled } from '@mui/material/styles';
import Typography from '@mui/material/Typography';

import ContentCard from '../components/ContentCard';
import Graph from '../components/Graph';
import styles from '../styles/styles.module.css';

const StatsBox = styled('div')`
  display: flex;
  width: 100%;
  justify-content: space-between;
  margin: 0;
`;

const StatsItem = styled('div')`
  flex: 1;
  text-align: center;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 5px;
`;

const SplitHeader = styled('div')`
  display: flex;
  width: 100%;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 10px;
`;

const Dashboard = ({ data, maxTemp }) => {

  const renderStats = (stats, label='') => {
    const getTempColour = (temp) => {
      if (temp >= maxTemp) {
        return styles.tempError;
      } else if (temp >= maxTemp - 2) {
        return styles.tempWarning;
      }
      return '';
    };
    
    return (
      <Box sx={{ display: 'flex', flexDirection: 'column', alignItems: 'center' }}>
        <SplitHeader>
          <Typography variant='h5'><strong>{label}</strong></Typography>
          { Object.keys(stats).length > 0 &&
            <Chip label={`${stats.data.length} data points`} color='secondary' variant='outlined'/>
          }
        </SplitHeader>
        { Object.keys(stats).length > 0 ?
          <StatsBox>
            <StatsItem>
              <Chip label="MINIMUM" variant="outlined"/>
              <div className={getTempColour(stats.min.temp)}>{stats.min.temp} °C</div>
            </StatsItem>
            <Divider orientation='vertical' flexItem sx={{ mx: 2 }}/>
            <StatsItem>
              <Chip label="AVERAGE" variant="outlined"/>
              <div className={getTempColour(stats.avg)}>{stats.avg} °C</div>
            </StatsItem>
            <Divider orientation='vertical' flexItem sx={{ mx: 2 }}/>
            <StatsItem>
              <Chip label="MAXIMUM" variant="outlined"/>
              <div className={getTempColour(stats.max.temp)}>{stats.max.temp} °C</div>
            </StatsItem>
          </StatsBox>
          :
          <i>No data available</i>
        }
      </Box>
    );
  };

  return (
    <>
      <ContentCard>
        { renderStats(data.statsToday, 'Today') }
      </ContentCard>
      <ContentCard>
        <Typography variant='h5'><strong>Last 24 Hours</strong></Typography>
        <Box sx={{ display: 'flex', flexDirection: 'column', alignItems: 'center' }}>
          { data.dataLast24Hours.length > 0 ?
            <Graph data={data.dataLast24Hours} maxTemp={maxTemp} brush/>
            :
            <i>No data available</i>
          }
        </Box>
      </ContentCard>
      <ContentCard>
        <Typography variant='h5'><strong>Average Temperature This Week</strong></Typography>
        { data.dataWeek.length > 0 ?
          <Graph data={data.dataWeek} maxTemp={maxTemp} domain='week'/>
          :
          <i>No data available</i>
        }
      </ContentCard>
      <ContentCard>
        <Typography variant='h5'><strong>Average Temperature This Year</strong></Typography>
        { data.dataYear.length > 0 ?
          <Graph data={data.dataYear} maxTemp={maxTemp} domain='year'/>
          :
          <i>No data available</i>
        }
      </ContentCard>
    </>
  );
};
export default Dashboard;