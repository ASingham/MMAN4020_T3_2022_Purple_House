import Box from '@mui/material/Box';
import TextField from '@mui/material/TextField';
import Typography from '@mui/material/Typography';
import moment from 'moment';
import { useState } from 'react';

import ContentCard from '../components/ContentCard';
import Graph from '../components/Graph';
import Table from '../components/Table';
import styles from '../styles/styles.module.css';

const DateSearch = ({ data, maxTemp }) => {
  const [date, setDate] = useState(moment().format('YYYY-MM-DD'));
  const dateData = data.getDataFromDate(date);
  
  return (
    <>
      <ContentCard>
        <Box className={styles.splitbox}>
          <Typography variant="h5">Date Search</Typography>
          <TextField
            id="date"
            label="Date"
            type="date"
            defaultValue={date}
            sx={{ width: 160 }}
            onChange={e => setDate(e.target.value)}
            color='secondary'
          />
        </Box>
        { dateData.length === 0 && <i>No data available</i> }
      </ContentCard>
      { dateData.length > 0 &&
        <>
          <ContentCard>
            <Graph data={dateData} maxTemp={maxTemp}/>
          </ContentCard>
          <ContentCard>
            <Table data={dateData} maxTemp={maxTemp}/>
          </ContentCard>
        </>
      }
    </>
  );
};
export default DateSearch;