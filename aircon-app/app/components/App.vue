<template>
    <Page>
        <ActionBar title="Welcome to NativeScript-Vue!"/>

        <GridLayout columns="*" rows="50, 50, 50, 50, 50" width="300" height="600" backgroundColor="white" >
          <GridLayout id="button_layout" columns="150, 150" rows="50" width="300" height="600" row="0" col="0" backgroundColor="white" >
            <Button id="on_button" text="ON" @tap="add" col="0" row="0"/>
            <Button id="off_button" text="OFF" @tap="add" col="1" row="0"/>
          </GridLayout>
          
  
          <TextField class="message" keyboardType="number"  col="0" row="2" v-model="inputTemp" />
          <Button id="set_button" text="SET" @tap="add" col="0" row="3"/>
        </GridLayout>
    </Page>
</template>

<script>
  export default {
    data() {
      return {
        inputTemp: 22
      }
    },

    methods: {
      add () {
        this.inputTemp += 1
      }
    }
  }
</script>

<style scoped>
  ActionBar {
    background-color: #53ba82;
    color: #ffffff;
  }

  Button#on_button {
    background-color: #1bbe36;
    color: rgb(241, 239, 239);
  }
  Button#off_button {
    background-color: #c03927;
    color: rgb(241, 239, 239);
  }
  GridLayout#button_layout {
    margin-top: 100px;
  }
  Button#set_button {
    background-color: #4f10b4;
    color: rgb(241, 239, 239);
  }

  .message {
    vertical-align: center;
    text-align: center;
    font-size: 20;
    color: #333333;
  }
</style>
