import Box from '@mui/material/Box';
import Paper from '@mui/material/Paper';

const ContentCard = ({ children }) => {
  return (
    <Paper elevation={4}>
      <Box p={3}>
        { children }
      </Box>
    </Paper>
  );
};

export default ContentCard;
