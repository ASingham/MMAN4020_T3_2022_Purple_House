import Box from '@mui/material/Box';
import Chip from '@mui/material/Chip';
import { styled } from '@mui/material/styles';
import Tooltip from '@mui/material/Tooltip';
import Typography from '@mui/material/Typography';
import { useState } from 'react';

import ContentCard from '../components/ContentCard';
import ContentWrapper from '../components/ContentWrapper';
import FileForm from '../components/FileForm';
import styles from '../styles/styles.module.css';
import Dashboard from './Dashboard';
import DateSearch from './DateSearch';

const FileBox = styled(Box)`
  min-width: 0;
  flex: 1;
  display: flex;
  justify-content: center;
  & > * {
    width: 80%;
  }
`;

const Home = ({ display, setDisplay, allData, setAllData, maxTemp }) => {
  const [selectedData, setSelectedData] = useState(0);
  const data = allData[selectedData];

  const renderMainContent = (disp, data) => {
    switch(disp) {
    case 'dashboard':
      return <Dashboard data={data} maxTemp={maxTemp}/>;
    case 'datesearch':
      return <DateSearch data={data} maxTemp={maxTemp}/>;
    default:
      return;
    }
  };

  const handleSubmit = (newData) => {
    setAllData(newData);
    setDisplay('dashboard');
    setSelectedData(0);
  };

  if (!allData.length) return (
    <ContentWrapper center>
      <ContentCard>
        <Box className={styles.verticalcontainer} sx={{ width: '100%' }}>
          <Typography align='center' variant='h5' sx={{ pb: 1 }}>AskSensors CSV File Upload</Typography>
          <Typography align='center' color='silver'>Upload up to 4 AskSensors CSV files</Typography>
          <FileForm onSubmit={handleSubmit}/>
        </Box>
      </ContentCard>
    </ContentWrapper>
  );

  return (
    <ContentWrapper>
      <Box sx={{ display: 'flex' }}>
        { allData.map((d, idx) => (
          <FileBox key={idx}>
            <Tooltip title={d.name} arrow>
              <Chip label={d.name} onClick={() => setSelectedData(idx)} color={idx === selectedData ? 'secondary' : 'default'} />
            </Tooltip>
          </FileBox>
        ))}
      </Box>
      { renderMainContent(display, data) }
    </ContentWrapper>
  );
};

export default Home;
