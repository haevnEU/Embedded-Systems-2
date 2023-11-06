import {
  Dialog,
  DialogTitle,
  DialogContent,
  DialogActions,
  Button,
  Slide,
  Box,
  TextField,
  InputAdornment,
  Typography,
} from "@mui/material";
import { TransitionProps } from "@mui/material/transitions";
import React, { useState } from "react";
import ApiService from "../../../api/api";
import { useDevices } from "../../../contexts/DeviceContext";
import InfoIcon from "@mui/icons-material/Info";

export interface ICreateDeviceDialogProps {
  open: boolean;
  onClose(): void;
}

const Transition = React.forwardRef(function Transition(
  props: TransitionProps & {
    children: React.ReactElement<any, any>;
  },
  ref: React.Ref<unknown>
) {
  return <Slide direction="up" ref={ref} {...props} />;
});

const CreateDeviceDialog = (props: ICreateDeviceDialogProps) => {
  const { refreshDevices } = useDevices();
  const [name, setName] = useState("");
  const [minCapacity, setMinCapacity] = useState(0);
  const [maxCapacity, setMaxCapacity] = useState(0);
  const [errorText, setErrorText] = useState("");
  const [showError, setShowError] = useState(false);

  const handleSave = async () => {
    setShowError(false);
    if (validateFields()) {
      var { success, error, file } = await ApiService.createDevice(
        name,
        minCapacity,
        maxCapacity
      );
      setErrorText(error);
      setShowError(!success);
      if (file) {
        const url = window.URL.createObjectURL(new Blob([file]));
        const link = document.createElement("a");
        link.href = url;
        link.setAttribute("download", `main.ino`);

        document.body.appendChild(link);
        link.click();
      }
      if (success) {
        setName("");
        setMinCapacity(0);
        setMaxCapacity(0);
        refreshDevices();
        props.onClose();
      }
    } else {
      setShowError(true);
      setErrorText("Name and/or capacities not valid!");
    }
  };

  const validateFields = () => {
    if (
      minCapacity < 0 ||
      maxCapacity <= 0 ||
      maxCapacity < minCapacity ||
      !name
    ) {
      return false;
    }
    return true;
  };

  return (
    <Dialog
      open={props.open}
      TransitionComponent={Transition}
      keepMounted
      onClose={props.onClose}
      aria-describedby="alert-dialog-slide-description"
    >
      <DialogTitle>{"Create device"}</DialogTitle>
      <DialogContent>
        <Box
          mt={1}
          display="flex"
          flexDirection="column"
          justifyContent="space-around"
          alignItems="center"
          rowGap="1em"
        >
          <TextField
            sx={{ width: "100%" }}
            onChange={(e) => setName(e.target.value)}
            required
            label="Device Name"
          />
          <TextField
            sx={{ width: "100%" }}
            onChange={(e) => setMinCapacity(+e.target.value)}
            required
            label="Min water capacity"
            InputProps={{
              endAdornment: <InputAdornment position="end">ml</InputAdornment>,
              inputProps: {
                min: 0,
                max: 100,
              },
            }}
          />
          <TextField
            sx={{ width: "100%" }}
            onChange={(e) => setMaxCapacity(+e.target.value)}
            required
            label="Max water capacity"
            InputProps={{
              endAdornment: <InputAdornment position="end">ml</InputAdornment>,
            }}
          />
          <Box display="flex" alignItems="center" columnGap="1em">
            <InfoIcon />
            <Typography display="inline">
              If a device is created a file "main.ino" will be returned. The
              code inside needs to be written to the new microcontroller.
            </Typography>
            <Typography display={showError ? "block" : "none"} color="error">
              {errorText}
            </Typography>
          </Box>
        </Box>
      </DialogContent>
      <DialogActions>
        <Button variant="outlined" color="info" onClick={props.onClose}>
          Cancel
        </Button>
        <Button variant="contained" color="secondary" onClick={handleSave}>
          Save
        </Button>
      </DialogActions>
    </Dialog>
  );
};

export default CreateDeviceDialog;
