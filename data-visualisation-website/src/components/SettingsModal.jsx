import Button from '@mui/material/Button';
import Dialog from '@mui/material/Dialog';
import DialogActions from '@mui/material/DialogActions';
import DialogContent from '@mui/material/DialogContent';
import DialogContentText from '@mui/material/DialogContentText';
import DialogTitle from '@mui/material/DialogTitle';
import TextField from '@mui/material/TextField';
import { useState } from 'react';



const SettingsModal = ({ open, setOpen, maxTemp, setMaxTemp }) => { 
  const [modalTemp, setModalTemp] = useState(maxTemp);
  
  const handleSave = () => {
    setMaxTemp(Number.parseFloat(modalTemp));
    setOpen(false);
  };

  return (
    <Dialog open={open} onClose={() => setOpen(false)}>
      <DialogTitle>
        Settings
      </DialogTitle>
      <DialogContent>
        <DialogContentText>
          Update the maximum temperature.
        </DialogContentText>
        <TextField 
          type="number"
          InputProps={{
            inputProps: { 
              max: 100, min: 0, step: 0.01
            }
          }}
          label='Max Temp'
          value={modalTemp}
          onChange={(e) => setModalTemp(Number.parseFloat(e.target.value))}
          sx={{ mt: 2, width: '100%' }}
        />
      </DialogContent>
      <DialogActions>
        <Button variant='outlined' color='success' onClick={handleSave}>
          Save
        </Button>
        <Button variant='outlined' color='error' onClick={() => setOpen(false)}>
          Close
        </Button> 
      </DialogActions>
    </Dialog>
  );
};
export default SettingsModal;